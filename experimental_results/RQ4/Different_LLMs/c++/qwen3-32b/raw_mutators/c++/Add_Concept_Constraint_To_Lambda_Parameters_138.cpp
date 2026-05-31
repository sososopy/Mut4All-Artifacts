//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Concept_Constraint_To_Lambda_Parameters_138
 */ 
class MutatorFrontendAction_138 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(138)

private:
    class MutatorASTConsumer_138 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_138(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Concept_Constraint_To_Lambda_Parameters_138.h"

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_138::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        // Check if the lambda is in the main file
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        // Get the closure type
        const CXXRecordDecl *Closure = LE->getClosureType()->getAsCXXRecordDecl();
        if (!Closure)
            return;

        // Find the operator() method
        for (const auto *Method : Closure->methods()) {
            if (Method->isOperatorCall()) {
                for (const auto *Param : Method->parameters()) {
                    if (Param->getType()->isAutoType()) {
                        // Get the source range of the type
                        const TypeLoc TypeLoc = Param->getTypeSourceInfo()->getTypeLoc();
                        SourceRange TypeRange = TypeLoc.getSourceRange();
                        // Replace 'auto' with 'std::input_iterator auto'
                        Rewrite.ReplaceText(TypeRange, "std::input_iterator auto");
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_138::MutatorASTConsumer_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}