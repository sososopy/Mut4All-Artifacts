//source file
#include "../include/overload_operator_with_varargs_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructWithOperator")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;

        std::string structName = RD->getNameAsString();
        std::string newOverload = "friend void operator,(" + structName + ", ...);";

        SourceLocation insertLoc = RD->getEndLoc().getLocWithOffset(-1);
        Rewrite.InsertText(insertLoc, "\nprivate:\n" + newOverload + "\n");
    }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        isStruct(), 
        hasMethod(hasOverloadedOperatorName(","))
    ).bind("StructWithOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}