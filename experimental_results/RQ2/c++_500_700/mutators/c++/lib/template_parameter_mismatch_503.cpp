//source file
#include "../include/Mutator_template_parameter_mismatch_503.h"

// ========================================================================================================
#define MUT503_OUTPUT 1

void MutatorFrontendAction_503::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        for (auto *spec : TD->specializations()) {
            if (spec->isCompleteDefinition()) {
                auto templateArgs = spec->getTemplateArgs().asArray();
                if (templateArgs.size() > 0) {
                    const auto &argType = templateArgs[0].getAsType();
                    if (!argType.isNull()) {
                        std::string originalTypeName = argType.getAsString();
                        std::string newTypeName = "ConvertibleType"; // Example: a type with a conversion constructor
                        templateTypeMap[originalTypeName] = newTypeName;

                        // Perform mutation on the source code text by applying string replacement
                        std::string specStr = stringutils::rangetoStr(*(Result.SourceManager), spec->getSourceRange());
                        size_t pos = specStr.find(originalTypeName);
                        if (pos != std::string::npos) {
                            specStr.replace(pos, originalTypeName.length(), newTypeName);
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(spec->getSourceRange()), specStr);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_503::MutatorASTConsumer_503::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}