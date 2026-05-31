//source file
#include "../include/mutator_modify_template_function_parameter_packs_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation()) {
            templateFunctions.push_back(FD);
            auto funcBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
            auto paramList = FD->getTemplateSpecializationArgs();
            if (paramList && paramList->size() > 1) {
                std::string incorrectCall = "func(";
                for (unsigned i = 0; i < paramList->size() - 1; ++i) {
                    incorrectCall += "1, ";
                }
                incorrectCall += ");";
                funcBody.insert(funcBody.find("{") + 1, "\n/*mut20*/" + incorrectCall + "\n");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), funcBody);
            }
        }
    }
}

void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}