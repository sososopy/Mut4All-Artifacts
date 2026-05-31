//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Class_Declaration_288
 */ 
class MutatorFrontendAction_288 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(288)

private:
    class MutatorASTConsumer_288 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_288(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_class_declaration_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find('{') == string::npos)
        return;
      if (content.find('}') == string::npos)
        return;
      if (content.find(';') == string::npos)
        return;
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto pos_l = content.find('{');
      auto pos_r = content.rfind('}');
      if (pos_r <= pos_l + 1)
        return;
      auto body = content.substr(pos_l + 1, pos_r - pos_l - 1);
      llvm::outs() << body << '\n';
      auto fields = llvm::split(body, ';');
      if (fields.size() == 0)
        return;
      int index = getrandom::getRandomIndex(fields.size() - 1);
      auto target = fields[index];
      llvm::outs() << target << '\n';
      if (target.find('(') != string::npos &&
          target.find(')') != string::npos) { // member function
        auto pos_l = target.find('(');
        auto pos_r = target.find(')');
        if (pos_r <= pos_l + 1)
          return;
        auto params = target.substr(pos_l + 1, pos_r - pos_l - 1);
        auto param_list = llvm::split(params, ',');
        if (param_list.size() == 0)
          return;
        int param_index = getrandom::getRandomIndex(param_list.size() - 1);
        auto target_param = param_list[param_index];
        if (target_param.find(' ') == string::npos)
          return;
        auto pos = target_param.find(' ');
        target_param.erase(pos, 1);
        param_list[param_index] = target_param;
        string new_params = "";
        for (auto param : param_list) {
          new_params += param + ',';
        }
        new_params.pop_back();
        target = target.replace(pos_l + 1, pos_r - pos_l - 1, new_params);
      } else { // member variable
        if (target.find(' ') == string::npos)
          return;
        auto pos = target.find(' ');
        target.erase(pos, 1);
      }
      fields[index] = target;
      string new_body = "";
      for (auto field : fields) {
        new_body += field + ';';
      }
      new_body.pop_back();
      content = content.replace(pos_l + 1, pos_r - pos_l - 1, new_body);
      content = "/*mut288*/" + content;
      llvm::outs() << content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}