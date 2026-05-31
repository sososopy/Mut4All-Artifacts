//source file
#include "../include/alter_template_template_parameter_661.h"

// ========================================================================================================
#define MUT661_OUTPUT 1

void MutatorFrontendAction_661::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateArgs = TD->getTemplateArgs().asArray();
        for (unsigned i = 0; i < templateArgs.size(); ++i) {
            if (templateArgs[i].getKind() == clang::TemplateArgument::Type) {
                auto typeStr = templateArgs[i].getAsType().getAsString();
                if (typeStr.find("decltype(&") != std::string::npos) {
                    std::string newTypeStr = "typename " + typeStr.substr(9, typeStr.size() - 10);
                    Rewrite.ReplaceText(TD->getSourceRange(), newTypeStr);
                }
            }
        }
    }
}

void MutatorFrontendAction_661::MutatorASTConsumer_661::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}