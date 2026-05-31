//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/OMPMatchers.h"

/**
 * OpenMP_ArraySection_Negative_349
 */ 
class MutatorFrontendAction_349 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(349)

private:
    class MutatorASTConsumer_349 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_349(Rewriter &R) : TheRewriter(R) {}
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
        std::string arrayName;
        SourceLocation arraySizeLoc;
    };
};

//source file
#include "../include/OpenMP_ArraySection_Negative_349.h"
#include "clang/AST/ASTMatchers.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"

// ========================================================================================================
#define MUT349_OUTPUT 1

void MutatorFrontendAction_349::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *arrayDecl = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!arrayDecl || !Result.Context->getSourceManager().isWrittenInMainFile(arrayDecl->getLocation()))
            return;

        arrayName = arrayDecl->getNameAsString();
        if (auto arrayTypeInfo = arrayDecl->getTypeSourceInfo()) {
            if (auto arrayType = arrayTypeInfo->getType().getTypePtr()->getAsArrayTypeUnsafe()) {
                if (auto sizeExpr = arrayType->getSizeExpr()) {
                    arraySizeLoc = sizeExpr->getSourceRange().getBegin();
                }
            }
        }

        if (arraySizeLoc.isValid()) {
            Rewrite.ReplaceText(arraySizeLoc, 0, "-1");
        }
    } else if (auto *ompDirective = Result.Nodes.getNodeAs<clang::OMPExecutableDirective>("ompDirective")) {
        if (!ompDirective || !Result.Context->getSourceManager().isWrittenInMainFile(ompDirective->getBeginLoc()))
            return;

        for (auto *clause : ompDirective->clauses()) {
            if (auto *dependClause = dyn_cast<OMPDependClause>(clause)) {
                for (auto *dep : dependClause->dependents()) {
                    if (auto *declRef = dyn_cast<DeclRefExpr>(dep)) {
                        if (declRef->getDecl()->getNameAsString() == arrayName) {
                            std::string newText = arrayName + "[:][:]";
                            Rewrite.ReplaceText(declRef->getSourceRange(), newText);
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_349::MutatorASTConsumer_349::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher arrayMatcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    DeclarationMatcher ompMatcher = pragmaOpenMPDirective(ompTaskDirective(hasDependClause(ompDependClause(hasArgument(declRefExpr(to(varDecl(equalsBoundNode("arrayDecl")))))))).bind("ompDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(ompMatcher, &callback);
    matchFinder.matchAST(Context);
}