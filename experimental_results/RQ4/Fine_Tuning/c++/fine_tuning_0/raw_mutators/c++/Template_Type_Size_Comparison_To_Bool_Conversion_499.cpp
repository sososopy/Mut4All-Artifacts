//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Type_Size_Comparison_To_Bool_Conversion_499
 */ 
class MutatorFrontendAction_499 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(499)

private:
    class MutatorASTConsumer_499 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_499(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Type_Size_Comparison_To_Bool_Conversion_499.h"

// ========================================================================================================
#define MUT499_OUTPUT 1

void MutatorFrontendAction_499::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (TD->isThisDeclarationADefinition() == false)
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto sizeof_pos = content.find("sizeof");
      while (sizeof_pos != string::npos) {
        auto lparen_pos = content.find('(', sizeof_pos);
        auto rparen_pos = content.find(')', sizeof_pos);
        auto target = content.substr(lparen_pos + 1,
                                     rparen_pos - lparen_pos - 1); // get T
        auto cmp = getrandom::getRandomIndex(2);
        string cmp_op = "";
        if (cmp == 0)
          cmp_op = ">";
        else if (cmp == 1)
          cmp_op = "<";
        else if (cmp == 2)
          cmp_op = "==";
        else
          assert(false && "Mut499: cmp should have 3 possible values!");
        content.insert(rparen_pos + 1, cmp_op + " 1");
        sizeof_pos = content.find("sizeof", sizeof_pos + 1);
      }
      content = "/*mut499*/" + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_499::MutatorASTConsumer_499::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}