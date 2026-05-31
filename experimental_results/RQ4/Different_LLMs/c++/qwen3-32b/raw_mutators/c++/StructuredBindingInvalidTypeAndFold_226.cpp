//header file
#pragma once
#include "Mutator_base.h"

/**
 * StructuredBindingInvalidTypeAndFold_226
 */ 
class MutatorFrontendAction_226 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(226)

private:
    class MutatorASTConsumer_226 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_226(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_StructuredBindingInvalidTypeAndFold_226.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *decompDecl = Result.Nodes.getNodeAs<clang::DecompositionDecl>("decomp")) {
        if (!decompDecl || !Result.Context->getSourceManager().isWrittenInMainFile(decompDecl->getLocation()))
            return;

        DeclStmt *declStmt = dyn_cast<DeclStmt>(decompDecl->getDeclContext());
        if (!declStmt)
            return;

        FunctionDecl *funcDecl = dyn_cast<FunctionDecl>(declStmt->getDeclContext());
        if (!funcDecl || !funcDecl->isTemplateInstantiation())
            return;

        Stmt *body = funcDecl->getBody();
        if (!body)
            return;

        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(body);
        if (!compoundStmt)
            return;

        llvm::ArrayRef<Stmt *> stmts = compoundStmt->body();
        size_t index = 0;
        for (; index < stmts.size(); ++index) {
            if (stmts[index] == declStmt)
                break;
        }

        if (index + 1 >= stmts.size())
            return;

        Stmt *nextStmt = stmts[index + 1];
        BinaryOperator *binOp = dyn_cast<BinaryOperator>(nextStmt);
        if (!binOp || binOp->getOpcode() != BO_Comma)
            return;

        SourceRange declStmtRange = declStmt->getSourceRange();
        std::string declStmtText = stringutils::rangetoStr(*Result.SourceManager, declStmtRange);

        size_t openBracket = declStmtText.find('[');
        size_t closeBracket = declStmtText.find(']');
        if (openBracket != std::string::npos && closeBracket != std::string::npos) {
            std::string newDeclText = declStmtText.substr(0, openBracket + 1) + "pack" + declStmtText.substr(closeBracket);
            Rewrite.ReplaceText(declStmtRange, newDeclText);
        }

        SourceRange binOpRange = binOp->getSourceRange();
        std::string newBinOpText = "(pack, ...);";
        Rewrite.ReplaceText(binOpRange, newBinOpText);
    }
}

void MutatorFrontendAction_226::MutatorASTConsumer_226::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = decompositionDecl(declContext(functionTemplateDecl())).bind("decomp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}