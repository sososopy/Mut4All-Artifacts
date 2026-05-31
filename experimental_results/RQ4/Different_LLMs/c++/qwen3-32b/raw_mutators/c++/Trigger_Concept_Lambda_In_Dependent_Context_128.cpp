//header file
#pragma once
#include "Mutator_base.h"

/**
 * Trigger_Concept_Lambda_In_Dependent_Context_128
 */ 
class MutatorFrontendAction_TriggerConceptLambdaInDependentContext_128 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(128)

private:
    class MutatorASTConsumer_128 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_128(Rewriter &R) : TheRewriter(R) {}
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
        bool conceptDeclared = false;
    };
};

//source file
#include "../include/Trigger_Concept_Lambda_In_Dependent_Context_128.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT128_OUTPUT 1

void MutatorFrontendAction_TriggerConceptLambdaInDependentContext_128::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = *Result.SourceManager;
    if (auto *CD = Result.Nodes.getNodeAs<ConceptDecl>("Concept")) {
        if (!SM.isWrittenInMainFile(CD->getLocation()))
            return;
        conceptDeclared = true;
    } else if (auto *FTD = Result.Nodes.getNodeAs<FunctionTemplateDecl>("FunctionTemplates")) {
        if (!SM.isWrittenInMainFile(FTD->getLocation()))
            return;
        if (!conceptDeclared) {
            SourceLocation loc = SM.getLocForStartOfFile(SM.getMainFileID());
            string conceptDecl = "template <typename> concept C = requires { []{}; };\n";
            Rewrite.InsertText(loc, conceptDecl, true, true);
            conceptDeclared = true;
        }
        
        TemplateParameterList *TPL = FTD->getTemplateParameters();
        if (!TPL || TPL->size() == 0)
            return;
        
        TemplateTypeParmDecl *TTP = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
        if (!TTP)
            return;
        
        string TParam = TTP->getNameAsString();
        if (TParam.empty())
            return;
        
        FunctionDecl *FD = FTD->getTemplatedDecl();
        if (!FD->hasBody())
            return;
        
        CompoundStmt *Body = dyn_cast<CompoundStmt>(FD->getBody());
        if (!Body)
            return;
        
        SourceLocation endLoc = Body->getRBraceLoc();
        string stmt = "C<" + TParam + ">;";
        stmt = "/*mut128*/" + stmt;
        Rewrite.InsertTextBefore(endLoc, stmt);
    }
}

void MutatorFrontendAction_TriggerConceptLambdaInDependentContext_128::MutatorASTConsumer_128::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcherConcept = conceptDecl().bind("Concept");
    DeclarationMatcher matcherFunction = functionTemplateDecl(hasBody(cxxFunctionDecl())).bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcherConcept, &callback);
    matchFinder.addMatcher(matcherFunction, &callback);
    matchFinder.matchAST(Context);
}