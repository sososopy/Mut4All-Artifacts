//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_Constraints_182
 */ 
class MutatorFrontendAction_182 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(182)

private:
    class MutatorASTConsumer_182 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_182(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameter_constraints_182.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        //Filter nodes in header files
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;
        
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        size_t requiresPos = declaration.find("requires");
        if (requiresPos != std::string::npos) {
            size_t endPos = declaration.find('{', requiresPos);
            if (endPos != std::string::npos) {
                std::string originalConstraint = declaration.substr(requiresPos, endPos - requiresPos);
                
                // Modify the constraint by adding a logical operation
                std::string newConstraint = originalConstraint + " && (sizeof(T) != 4)";
                declaration.replace(requiresPos, endPos - requiresPos, newConstraint);
                
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), declaration);
            }
        }
    }
}
  
void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        hasRequiresClause()
    ).bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}