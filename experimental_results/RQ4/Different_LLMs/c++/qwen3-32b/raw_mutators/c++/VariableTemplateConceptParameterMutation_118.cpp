//header file
#pragma once
#include "Mutator_base.h"

/**
 * VariableTemplateConceptParameterMutation_118
 */ 
class MutatorFrontendAction_VariableTemplateConceptParameterMutation_118 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(VariableTemplateConceptParameterMutation_118)

private:
    class MutatorASTConsumer_VariableTemplateConceptParameterMutation_118 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_VariableTemplateConceptParameterMutation_118(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/VariableTemplateConceptParameterMutation_118.h"

// ========================================================================================================
#define MUTVariableTemplateConceptParameterMutation_118_OUTPUT 1

void MutatorFrontendAction_VariableTemplateConceptParameterMutation_118::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VarTemplate = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
        // Filter nodes in header files
        if (!VarTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(VarTemplate->getLocation()))
            return;

        // Get the requires clause
        clang::RequiresExpr *RE = VarTemplate->getRequiresClause();
        if (!RE)
            return;

        // Get the constraint expression
        clang::Expr *ConstraintExpr = RE->getConstraintExpr();
        if (!ConstraintExpr)
            return;

        // Get the source code for the constraint
        SourceManager &SM = Result.Context->getSourceManager();
        std::string ConstraintStr = stringutils::rangetoStr(SM, ConstraintExpr->getSourceRange());

        // Get the template parameters
        clang::TemplateParameterList *TPL = VarTemplate->getTemplateParameters();
        for (const auto *Param : TPL->asArray()) {
            if (const auto *TTP = clang::dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                // Replace "typename T" or "class T" with "C T"
                SourceRange TTPRange = TTP->getSourceRange();
                std::string NewParam = ConstraintStr + " " + TTP->getNameAsString();
                Rewrite.ReplaceText(TTPRange, NewParam);
            }
        }

        // Remove the requires clause
        SourceRange RERange = RE->getSourceRange();
        Rewrite.RemoveText(RERange);
    }
}
  
void MutatorFrontendAction_VariableTemplateConceptParameterMutation_118::MutatorASTConsumer_VariableTemplateConceptParameterMutation_118::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = varTemplateDecl(hasRequiresClause()).bind("VarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}