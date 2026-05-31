//source file
#include "../include/empty_template_parameter_pack_in_function_return_type_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        const auto *TemplateArgs = FD->getTemplateSpecializationArgs();
        if (TemplateArgs && TemplateArgs->size() == 0) {
            std::string funcCall = FD->getQualifiedNameAsString() + "<>::func<0>();";
            Rewrite.InsertTextAfterToken(FD->getEndLoc(), "\n" + funcCall + "\n");
        }
    }
}

void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(isTemplateInstantiation()).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}