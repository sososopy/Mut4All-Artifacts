//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Array_With_Flexible_Array_322
 */ 
class MutatorFrontendAction_322 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(322)

private:
    class MutatorASTConsumer_322 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_322(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Array_With_Flexible_Array_322.h"

// ========================================================================================================
#define MUT322_OUTPUT 1

void MutatorFrontendAction_322::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("arrayMember")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        //Get the source code text of target node
        auto text = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t openBracket = text.find('[');
        if (openBracket != std::string::npos) {
            size_t closeBracket = text.find(']', openBracket);
            if (closeBracket != std::string::npos) {
                // Replace the size part with empty
                std::string newText = text.substr(0, openBracket + 1) + 
                                      text.substr(closeBracket);
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(FD->getSourceRange(), newText);
            }
        }
    }
}
  
void MutatorFrontendAction_322::MutatorASTConsumer_322::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasType(arrayType()), hasParent(unionDecl())).bind("arrayMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}