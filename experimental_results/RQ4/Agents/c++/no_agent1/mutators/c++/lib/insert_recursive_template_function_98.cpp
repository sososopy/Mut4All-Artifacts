//source file
#include "../include/insert_recursive_template_function_98.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionName = FD->getNameAsString();
            auto returnType = FD->getReturnType().getAsString();
            auto templateFunction = "template<int N>\n" + returnType + " " + functionName + "_recursive() {\n"
                                    "if constexpr (N > 0) return " + functionName + "_recursive<N-1>();\n"
                                    "else return " + functionName + "();\n}\n";

            auto functionSourceRange = FD->getSourceRange();
            Rewrite.InsertTextBefore(functionSourceRange.getBegin(), "/*mut98*/" + templateFunction);
        }
    }
}

void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}