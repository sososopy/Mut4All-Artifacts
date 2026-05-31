//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Auto_Type_In_Struct_334
 */ 
class MutatorFrontendAction_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(334)

private:
    class MutatorASTConsumer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_334(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_auto_type_in_struct_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("autoMember")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        if (VD->getType()->isDependentType() || !VD->hasInit())
            return;

        //Get the initializer and infer the type
        const Expr *initExpr = VD->getInit();
        QualType inferredType = initExpr->getType();

        //Perform mutation on the source code text by applying string replacement
        std::string newTypeName = inferredType.getAsString();
        std::string originalText = VD->getTypeSourceInfo()->getTypeLoc().getSourceRange().printToString(*Result.SourceManager);
        std::string replacementText = newTypeName + " " + VD->getNameAsString();

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(VD->getTypeSourceInfo()->getTypeLoc().getSourceRange(), replacementText);
    }
}

void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(autoType()), isCXXInstance()).bind("autoMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}