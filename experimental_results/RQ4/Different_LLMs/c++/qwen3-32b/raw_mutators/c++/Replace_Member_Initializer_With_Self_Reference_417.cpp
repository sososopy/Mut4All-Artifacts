//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Initializer_With_Self_Reference_417
 */ 
class MutatorFrontendAction_417 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(417)

private:
    class MutatorASTConsumer_417 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_417(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> memberNames;
    };
};

//source file
#include "../include/Replace_Member_Initializer_With_Self_Reference_417.h"

// ========================================================================================================
#define MUT417_OUTPUT 1

void MutatorFrontendAction_417::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("DependentField")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getType()->isDependentType()) {
            // Get the member name
            std::string memberName = FD->getNameAsString();
            // Get the source code text of the initializer
            if (FD->hasInClassInitializer()) {
                // Replace the initializer with a self-reference
                std::string replacement = memberName;
                // Perform mutation on the source code text by applying string replacement
                SourceLocation initLoc = FD->getInClassInitializer()->getSourceRange().getBegin();
                Rewrite.ReplaceText(initLoc, 0, " = " + replacement);
            } else {
                // Add a self-reference initializer
                std::string replacement = " = " + memberName;
                Rewrite.InsertText(FD->getSourceRange().getEnd(), replacement);
            }
        } else {
            // If no dependent member exists, add a redundant member
            std::string redundantMember = "int y = y;";
            Rewrite.InsertText(FD->getSourceRange().getEnd(), "\n    " + redundantMember);
        }
    }
}
  
void MutatorFrontendAction_417::MutatorASTConsumer_417::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = fieldDecl(hasType(isDependentType()), inClass).bind("DependentField");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}