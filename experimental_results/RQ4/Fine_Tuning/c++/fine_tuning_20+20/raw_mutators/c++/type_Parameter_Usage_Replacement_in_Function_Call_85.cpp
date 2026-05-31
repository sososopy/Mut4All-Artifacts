//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Non-type_Parameter_Usage_Replacement_in_Function_Call_85
 */ 
class MutatorFrontendAction_85 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(85)

private:
    class MutatorASTConsumer_85 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_85(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Non-type_Parameter_Usage_Replacement_in_Function_Call_85.h"

// ========================================================================================================
#define MUT85_OUTPUT 1

void MutatorFrontendAction_85::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("Call")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto callee = MT->getDirectCallee();
      if (callee == nullptr)
        return;
      auto callee_name = callee->getNameAsString();
      auto callee_template = callee->getDescribedTemplate();
      if (callee_template == nullptr)
        return;
      auto params = callee_template->getTemplateParameters();
      if (params == nullptr)
        return;
      //Perform mutation on the source code text by applying string replacement
      for (auto param : *params) {
        if (auto ntparam = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          auto param_type = ntparam->getType();
          llvm::outs() << param_type.getAsString() << '\n';
          if (param_type->isIntegralOrEnumerationType()) {
            if (content.find('<') != string::npos &&
                content.find('>') != string::npos) {
              auto template_args =
                  content.substr(content.find('<') + 1,
                                 content.find('>') - content.find('<') - 1);
              llvm::outs() << template_args << '\n';
              if (template_args.find(',') != string::npos) {
                auto args = stringutils::split(template_args, ',');
                for (auto arg : args) {
                  llvm::outs() << arg << '\n';
                  if (arg.find_first_not_of("0123456789") == string::npos) {
                    llvm::outs() << "literal\n";
                    continue;
                  } else {
                    llvm::outs() << "non-literal\n";
                    auto casted_arg = "static_cast<int>(" + arg + ")";
                    content.replace(content.find(arg), arg.length(),
                                    casted_arg);
                  }
                }
              } else {
                if (template_args.find_first_not_of("0123456789") ==
                    string::npos) {
                  llvm::outs() << "literal\n";
                } else {
                  llvm::outs() << "non-literal\n";
                  auto casted_arg =
                      "static_cast<int>(" + template_args + ")";
                  content.replace(content.find(template_args),
                                  template_args.length(), casted_arg);
                }
              }
            }
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_85::MutatorASTConsumer_85::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr().bind("Call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}