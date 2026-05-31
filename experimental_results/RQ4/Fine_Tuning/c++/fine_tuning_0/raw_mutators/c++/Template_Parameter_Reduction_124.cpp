//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Reduction_124
 */ 
class MutatorFrontendAction_124 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(124)

private:
    class MutatorASTConsumer_124 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_124(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Parameter_Reduction_124.h"

// ========================================================================================================
#define MUT124_OUTPUT 1

void MutatorFrontendAction_124::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("MultiTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto params = MT->getTemplateParameters();
      if (params->size() <= 1)
        return;
      auto param = params->getParam(0);
      auto param_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               param->getSourceRange());
      for (size_t i = 1; i < params->size(); ++i) {
        auto cur_param = params->getParam(i);
        auto cur_param_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                     cur_param->getSourceRange());
        llvm::outs() << cur_param_str << '\n';
        int dice = getrandom::getRandomIndex(1);
        if (dice == 0)
          param_str += ", " + cur_param_str;
      }
      llvm::outs() << param_str << '\n';
      auto template_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
      auto template_head = template_str.substr(
          0, template_str.find('<') + 1); // template< 的长度是9
      auto template_tail = template_str.substr(template_str.rfind('>'));
      auto new_template_str = template_head + param_str + template_tail;
      new_template_str = "/*mut124*/" + new_template_str;
      llvm::outs() << new_template_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          new_template_str);
    }
}
  
void MutatorFrontendAction_124::MutatorASTConsumer_124::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(has(templateTypeParmDecl())).bind("MultiTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}