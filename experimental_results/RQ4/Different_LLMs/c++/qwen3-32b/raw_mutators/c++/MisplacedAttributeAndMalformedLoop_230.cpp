//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Stmt.h"

/**
 * MisplacedAttributeAndMalformedLoop_230
 */ 
class MutatorFrontendAction_230 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(230)
private:
    class MutatorASTConsumer_230 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_230(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl*> templateParams;
    };
};

//source file
#include "../include/misplaced_attribute_and_malformed_loop_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_230::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TTP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateParam")) {
        if (!TTP || !Result.Context->getSourceManager().isWrittenInMainFile(TTP->getLocation()))
            return;
        
        if (TTP->hasDefaultArgument()) {
            std::string attribute = "[[deprecated]]";
            std::string original = Rewrite.getRewrittenText(TTP->getSourceRange());
            
            // Insert attribute before parameter name
            std::string mutated = attribute + " " + original;
            Rewrite.ReplaceText(TTP->getSourceRange(), mutated);
        }
    } 
    else if (auto *FRS = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("forRange")) {
        if (!FRS || !Result.Context->getSourceManager().isWrittenInMainFile(FRS->getBeginLoc()))
            return;
        
        auto rangeInit = FRS->getRangeInit();
        if (!rangeInit)
            return;
        
        std::string original = Rewrite.getRewrittenText(rangeInit->getSourceRange());
        std::string mutated = "createRange<typename T::InvalidMember>()";
        
        // Replace range expression with malformed type
        Rewrite.ReplaceText(rangeInit->getSourceRange(), mutated);
    }
}
  
void MutatorFrontendAction_230::MutatorASTConsumer_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = templateTypeParmDecl(hasDefaultArgument()).bind("templateParam");
    DeclarationMatcher matcher2 = cxxForRangeStmt(hasRangeInit(declRefExpr(to(templateTypeParmDecl())))).bind("forRange");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}