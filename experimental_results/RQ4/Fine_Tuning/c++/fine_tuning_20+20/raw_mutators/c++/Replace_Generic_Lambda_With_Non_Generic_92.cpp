//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Generic_Lambda_With_Non_Generic_92
 */ 
class MutatorFrontendAction_92 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(92)

private:
    class MutatorASTConsumer_92 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_92(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_generic_lambda_with_non_generic_92.h"

// ========================================================================================================
#define MUT92_OUTPUT 1

void MutatorFrontendAction_92::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      auto lambda_pos = content.find('[');
      if (lambda_pos == string::npos)
        return;
      auto param_pos = content.find('(', lambda_pos);
      if (param_pos == string::npos)
        return;
      auto param_end = content.find(')', param_pos);
      if (param_end == string::npos)
        return;
      auto param_content = content.substr(param_pos + 1, param_end - param_pos - 1);
      llvm::outs() << param_content << '\n';
      vector<string> param_list;
      string cur_param;
      for (char c : param_content) {
        if (c == ',') {
          param_list.push_back(cur_param);
          cur_param = "";
        } else
          cur_param += c;
      }
      if (!cur_param.empty())
        param_list.push_back(cur_param);
      for (auto &param : param_list) {
        param = stringutils::trim(param);
        llvm::outs() << param << '\n';
      }
      auto body_pos = content.find('{', param_end);
      if (body_pos == string::npos)
        return;
      if (content.find("auto", lambda_pos) > body_pos)
        return;
      if (param_list.size() == 0)
        return;
      string new_param = "";
      for (size_t i = 0; i < param_list.size(); ++i) {
        if (i)
          new_param += ",";
        new_param += param_list[i] + "&& arg" + std::to_string(i);
      }
      llvm::outs() << new_param << '\n';
      content.replace(param_pos + 1, param_end - param_pos - 1, new_param);
      //Perform mutation on the source code text by applying string replacement
      content = "/*mut92*/" + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_92::MutatorASTConsumer_92::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}