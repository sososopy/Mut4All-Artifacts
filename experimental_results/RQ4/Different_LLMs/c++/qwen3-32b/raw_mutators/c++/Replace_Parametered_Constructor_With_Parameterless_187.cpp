//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Parametered_Constructor_With_Parameterless_187
 */ 
class MutatorFrontendAction_187 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(187)

private:
    class MutatorASTConsumer_187 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_187(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Parametered_Constructor_With_Parameterless_187.h"

// ========================================================================================================
#define MUT187_OUTPUT 1

void MutatorFrontendAction_187::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        //Get the source code text of target node
        auto original = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t open_paren = original.find('(');
        if (open_paren == std::string::npos) return;
        size_t close_paren = original.find(')', open_paren);
        if (close_paren == std::string::npos) return;
        std::string new_decl = original.substr(0, open_paren) + "()";
        new_decl += "{}";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), new_decl);
    }
}
  
void MutatorFrontendAction_187::MutatorASTConsumer_187::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(hasParameter(0, anything())).bind("constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}