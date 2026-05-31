//source file
#include "../include/atomic_type_mismatch_in_comparisons_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getIfLoc()))
            return;

        auto *BO = llvm::dyn_cast<clang::BinaryOperator>(IF->getCond());
        if (!BO || BO->getOpcode() != clang::BO_EQ)
            return;

        auto *AtomicType = Result.Nodes.getNodeAs<clang::VarDecl>("atomicVar");
        auto *OtherType = Result.Nodes.getNodeAs<clang::VarDecl>("otherVar");

        if (!AtomicType || !OtherType)
            return;

        std::string atomicVarName = AtomicType->getNameAsString();
        std::string otherVarName = OtherType->getNameAsString();

        std::string replacement = "if (" + atomicVarName + " == intVar)";
        Rewrite.ReplaceText(IF->getCond()->getSourceRange(), replacement);
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto atomicMatcher = varDecl(hasType(recordDecl(hasName("std::atomic")))).bind("atomicVar");
    auto otherMatcher = varDecl(unless(hasType(recordDecl(hasName("std::atomic"))))).bind("otherVar");
    auto ifStmtMatcher = ifStmt(hasCondition(binaryOperator(hasOperatorName("==")))).bind("ifStmt");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(atomicMatcher, &callback);
    matchFinder.addMatcher(otherMatcher, &callback);
    matchFinder.addMatcher(ifStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}