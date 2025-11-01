//source file
#include "../include/null_pointer_dereference_in_base_class_handling_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXBaseSpecifier>("BaseSpecifier")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getBeginLoc()))
            return;

        auto baseType = Base->getType();
        auto baseDecl = baseType->getAsCXXRecordDecl();
        if (!baseDecl)
            return;

        std::string mutation = "const CXXRecordDecl* baseDecl = nullptr;\n";
        mutation += "#ifdef MUT190_OUTPUT\n";
        mutation += "if (baseType->isDependentType()) baseDecl = nullptr;\n";
        mutation += "else baseDecl = baseType->getAsCXXRecordDecl();\n";
        mutation += "#endif\n";

        SourceLocation insertLoc = Base->getBeginLoc();
        Rewrite.InsertText(insertLoc, mutation, true, true);
    }
}

void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(has(cxxBaseSpecifier().bind("BaseSpecifier")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}