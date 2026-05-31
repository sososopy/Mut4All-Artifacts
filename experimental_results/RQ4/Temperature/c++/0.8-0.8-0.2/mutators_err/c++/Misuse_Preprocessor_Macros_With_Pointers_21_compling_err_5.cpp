// header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_preprocessor_macros_with_pointers_21
 */

class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;

    private:
        Rewriter &TheRewriter;
    };

    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);

    private:
        Rewriter &Rewrite;
        const clang::FunctionDecl *functionDecl = nullptr;
    };
};

// source file
#include "misuse_preprocessor_macros_with_pointers_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Ensure the function takes a struct pointer
        if (FD->getNumParams() == 0)
            return;
        const ParmVarDecl *param = FD->getParamDecl(0);
        if (!param->getType()->isPointerType())
            return;
        const auto *recordType = param->getType()->getPointeeType()->getAs<clang::RecordType>();
        if (!recordType)
            return;
        
        functionDecl = FD;
    } else if (const auto *DS = Result.Nodes.getNodeAs<clang::DeclStmt>("DeclStmt")) {
        if (!functionDecl || !Result.Context->getSourceManager().isWrittenInMainFile(DS->getBeginLoc()))
            return;

        // Get the source code text of target node
        auto declText = stringutils::rangetoStr(*(Result.SourceManager), DS->getSourceRange());

        // Apply string replacement
        std::string newDecl = "constexpr int invalid_constexpr = max(5, " + functionDecl->getParamDecl(0)->getNameAsString() + "->member);";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DS->getSourceRange()), newDecl);
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    // Define ASTMatcher to identify the function declaration taking a struct pointer
    DeclarationMatcher functionMatcher = functionDecl(
        hasParameter(0, hasType(pointerType(pointee(recordType(hasDeclaration(cxxRecordDecl())))))),
        hasDescendant(declStmt(hasDescendant(declRefExpr(to(varDecl(hasType(isInteger())).bind("VarDecl")))).bind("DeclStmt")))
    ).bind("FunctionDecl");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}