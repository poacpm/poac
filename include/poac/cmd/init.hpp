#ifndef POAC_CMD_INIT_HPP
#define POAC_CMD_INIT_HPP

// std
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

// external
#include <fmt/core.h>
#include <mitama/result/result.hpp>
#include <plog/Log.h>

// internal
#include <poac/cmd/new.hpp>
#include <poac/core/validator.hpp>
#include <poac/util/termcolor2/termcolor2.hpp>
#include <poac/util/termcolor2/literals_extra.hpp>

namespace poac::cmd::init {
    struct Options {
        _new::ProjectType type;
    };

    [[nodiscard]] mitama::result<void, std::string>
    init(std::string_view package_name, init::Options&& opts) {
        PLOG_VERBOSE << "Creating ./poac.toml";
        std::ofstream ofs_config("poac.toml");
        switch (opts.type) {
            case _new::ProjectType::Bin:
                ofs_config << _new::files::poac_toml(package_name);
                break;
            case _new::ProjectType::Lib:
                ofs_config << _new::files::poac_toml(package_name);
                break;
        }

        using termcolor2::color_literals::operator""_bold_green;
        PLOG_INFO << fmt::format(
            "{:>25} {} `{}` package",
            "Created"_bold_green,
            opts.type,
            package_name
        );
        return mitama::success();
    }

    [[nodiscard]] mitama::result<void, std::string>
    exec(Options&& opts) {
        if (core::validator::require_config_exists().is_ok()) {
            return mitama::failure(
                "`poac init` cannot run on existing poac packages"
            );
        }

        const std::string package_name = std::filesystem::current_path().stem().string();
        PLOG_VERBOSE << fmt::format(
            "Validating the package name `{}`", package_name
        );
        MITAMA_TRY(core::validator::valid_package_name(package_name));

        return init(package_name, std::move(opts));
    }
} // end namespace

#endif // !POAC_CMD_INIT_HPP
