//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Attributed_Lambda_In_Templated_Function_483
 */ 
class MutatorFrontendAction_483 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(483)

private:
    class MutatorASTConsumer_483 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_483(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/Add_Attributed_Lambda_In_Templated_Function_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isThisDeclarationADefinition() == false)
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto lambda =
          "\n/*mut483*/auto l = []() __attribute__((pcs(\"aapcs-vfp\"))) {};";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), lambda);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      cur_templates.push_back(DL);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>(
                   "CallExpr")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callee = CL->getDirectCallee();
      if (callee == nullptr)
        return;
      auto callee_name = callee->getNameAsString();
      for (auto target : cur_templates) {
        if (target->getNameAsString() == callee_name) {
          auto args = CL->getNumArgs();
          if (args != 0)
            return;
          auto specs = target->specializations();
          if (specs.empty())
            return;
          auto spec = specs[0];
          auto types = spec->getTemplateArgs().asArray();
          if (types.size() == 0)
            return;
          auto type = types[0].getAsType();
          //Perform mutation on the source code text by applying string replacement
          auto ins = "/*mut483*/<" + type.getAsString() + ">";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CL->getBeginLoc(), 0, ins);
        }
      }
    }
  }

void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto matcher = functionTemplateDecl().bind("Templates");
    auto call_matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}