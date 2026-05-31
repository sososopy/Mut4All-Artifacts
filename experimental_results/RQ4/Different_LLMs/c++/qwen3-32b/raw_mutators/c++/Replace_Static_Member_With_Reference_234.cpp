//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_With_Reference_234
 */ 
class MutatorFrontendAction_234 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(234)
private:
    class MutatorASTConsumer_234 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_234(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Member_With_Reference_234.h"

// ========================================================================================================
#define MUT234_OUTPUT 1

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        //Check if it's a static member
        if (!FD->isStaticStorageClass())
            return;
        //Get the typeLoc
        auto *TL = Result.Nodes.getNodeAs<clang::TypeLoc>("typeLoc");
        if (!TL)
            return;
        //Get the source range of the type
        SourceRange typeRange = TL->getSourceRange();
        if (typeRange.isInvalid())
            return;
        //Get the source code text of the type
        std::string originalType = stringutils::rangetoStr(*Result.SourceManager, typeRange);
        //Perform mutation by appending &
        std::string newType = originalType + "&";
        //Replace the original type with the mutated one
        Rewrite.ReplaceText(typeRange, newType);
    }
}
  
void MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = fieldDecl(hasStaticStorageDuration(), hasType(typeLoc().bind("typeLoc"))).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}