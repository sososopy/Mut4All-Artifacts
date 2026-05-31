//source file
#include "../include/replace_function_body_with_throw_exception_89.h"

// ========================================================================================================
#define MUT89_OUTPUT 1

void MutatorFrontendAction_89::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionBody = dyn_cast<CompoundStmt>(FD->getBody());
            if (!functionBody)
                return;

            std::string exceptionCode = "{\n/*mut89*/throw std::runtime_error(\"Mutated exception\");\n}";
            Rewrite.ReplaceText(functionBody->getSourceRange(), exceptionCode);
        }
    }
}

void MutatorFrontendAction_89::MutatorASTConsumer_89::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}