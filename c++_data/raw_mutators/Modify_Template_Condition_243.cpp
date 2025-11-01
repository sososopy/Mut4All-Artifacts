//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Condition_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)

private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Condition_243.h"

// ========================================================================================================
#define MUT243_OUTPUT 1

void MutatorFrontendAction_243::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("enable_if<");
      if (pos != std::string::npos) {
          size_t endPos = declaration.find(",", pos);
          if (endPos != std::string::npos) {
              declaration.replace(pos, endPos - pos, "enable_if<Foo<T>::non_existent_member");
          }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_243::MutatorASTConsumer_243::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(typeTraitExpr())).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}