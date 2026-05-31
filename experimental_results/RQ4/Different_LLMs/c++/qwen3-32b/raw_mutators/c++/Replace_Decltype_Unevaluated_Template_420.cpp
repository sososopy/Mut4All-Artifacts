//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_Unevaluated_Template_420
 */ 
class MutatorFrontendAction_420 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(420)

private:
    class MutatorASTConsumer_420 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_420(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Decltype_Unevaluated_Template_420.h"

// ========================================================================================================
#define MUT420_OUTPUT 1

void MutatorFrontendAction_420::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (!VD->getInit() || !VD->getTypeSourceInfo()) return;

        //Get the initializer expression
        Expr *init = VD->getInit()->IgnoreImplicit();
        if (!init) return;

        //Get the source range of the initializer
        SourceRange initRange = init->getSourceRange();
        if (initRange.isInvalid()) return;

        //Get the source text of the initializer
        std::string initStr = stringutils::rangetoStr(*Result.SourceManager, initRange);
        if (initStr.empty()) return;

        //Generate the new type string
        std::string newType = "decltype(" + initStr + "::InnerType::value())";

        //Get the source range of the original type
        TypeSourceInfo *TSI = VD->getTypeSourceInfo();
        SourceRange typeRange = TSI->getTypeLoc().getSourceRange();

        //Replace the original type with the new type
        Rewrite.ReplaceText(typeRange, newType);
    }
}
  
void MutatorFrontendAction_420::MutatorASTConsumer_420::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(), anyOf(hasType(qualType(isAuto())), hasType(qualType()))).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}