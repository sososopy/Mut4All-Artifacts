//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Unconstrained_Variable_Template_With_Constrained_357
 */ 
class MutatorFrontendAction_357 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(357)
private:
    class MutatorASTConsumer_357 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_357(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Unconstrained_Variable_Template_With_Constrained_357.h"

// ========================================================================================================
#define MUT357_OUTPUT 1

void MutatorFrontendAction_357::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VT = Result.Nodes.getNodeAs<clang::VariableTemplateDecl>("VarTemplate")) {
        if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(VT->getLocation()))
            return;

        std::string varTemplateName = VT->getNameAsString();
        std::string conceptName = "Concept_357_" + varTemplateName;

        SourceLocation insertLoc = VT->getLocation();
        SourceManager &SM = Result.Context->getSourceManager();

        // Insert the concept declaration before the variable template
        std::string conceptDecl = "template<typename T>\nconcept " + conceptName + " = true;\n";
        Rewrite.InsertText(insertLoc, conceptDecl, true, true);

        // Modify the variable template's template parameters
        auto *TemplatedDecl = VT->getTemplatedDecl();
        auto *TemplateParams = TemplatedDecl->getTemplateParameters();
        if (!TemplateParams)
            return;

        auto paramRange = TemplateParams->getSourceRange();
        Rewrite.ReplaceText(paramRange, conceptName + " T");
    }
}
  
void MutatorFrontendAction_357::MutatorASTConsumer_357::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = variableTemplateDecl().bind("VarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}