//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_With_List_Init_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_decltype_with_list_init_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string replacement = "auto _ = " + VD->getType().getAsString() + "{}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(decltypeType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}