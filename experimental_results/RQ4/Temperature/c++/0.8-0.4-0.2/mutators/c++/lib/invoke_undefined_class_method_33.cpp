//source file
#include "../include/Invoke_Undefined_Class_Method_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithUndefinedMethod")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        for (auto *method : CD->methods()) {
            if (!method->hasBody()) {
                std::string className = CD->getNameAsString();
                std::string methodName = method->getNameAsString();
                std::string invocation = className + " instance;\ninstance." + methodName + "();\n";
                Rewrite.InsertTextAfterToken(CD->getEndLoc(), "\n/*mut33*/" + invocation);
                break;
            }
        }
    }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxMethodDecl(unless(hasBody(stmt()))))).bind("ClassWithUndefinedMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}