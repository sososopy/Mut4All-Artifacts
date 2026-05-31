//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Template_Arguments_And_Switch_Cases_In_Templated_Lambda_Operator_Call_436
 */ 
class MutatorFrontendAction_436 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(436)

private:
    class MutatorASTConsumer_436 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_436(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::vector<string>> lambda_cases;
    };
};

//source file
#include "../include/swap_template_arguments_and_switch_cases_in_templated_lambda_operator_call_436.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (!DL->hasTemplateParameters())
        return;
      auto params = DL->getTemplateParameterList(0);
      if (params->size() != 1)
        return;
      auto param = params->getParam(0);
      if (!param->isTemplateParameter())
        return;
      auto paramstr = stringutils::rangetoStr(*(Result.SourceManager),
                                              param->getSourceRange());
      if (paramstr != "typename t" && paramstr != "int t")
        return;
      auto body = DL->getBody();
      auto bodystr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      if (bodystr.find("switch") == string::npos)
        return;
      if (bodystr.find("operator()") == string::npos)
        return;
      llvm::outs() << "lambda body: " << bodystr << '\n';
      std::vector<string> cases;
      auto case_pos = bodystr.find("case");
      while (case_pos != string::npos) {
        auto case_end = bodystr.find(':', case_pos);
        if (case_end == string::npos)
          break;
        auto case_value = bodystr.substr(case_pos + 4, case_end - case_pos - 4);
        llvm::outs() << "case value: " << case_value << '\n';
        cases.push_back(case_value);
        case_pos = bodystr.find("case", case_end);
      }
      lambda_cases.push_back(cases);
    } else if (auto *CS = Result.Nodes.getNodeAs<clang::CaseStmt>("Case")) {
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      auto case_value = stringutils::rangetoStr(*(Result.SourceManager),
                                                CS->getLHS()->getSourceRange());
      llvm::outs() << "case value: " << case_value << '\n';
      auto case_body = CS->getSubStmt();
      auto case_body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                   case_body->getSourceRange());
      llvm::outs() << "case body: " << case_body_str << '\n';
      if (case_body_str.find("operator()") == string::npos)
        return;
      auto op_pos = case_body_str.find("operator()");
      auto op_end = case_body_str.find('>', op_pos);
      if (op_end == string::npos)
        return;
      auto op_args = case_body_str.substr(op_pos + 10, op_end - op_pos - 10);
      llvm::outs() << "operator args: " << op_args << '\n';
      std::vector<string> args;
      auto arg_pos = op_args.find(',');
      while (arg_pos != string::npos) {
        args.push_back(op_args.substr(0, arg_pos));
        op_args = op_args.substr(arg_pos + 1);
        arg_pos = op_args.find(',');
      }
      args.push_back(op_args);
      llvm::outs() << "args size: " << args.size() << '\n';
      if (args.size() != 3)
        return;
      auto lambda_index = lambda_cases.size() - 1;
      auto lambda_case = lambda_cases[lambda_index];
      if (lambda_case.size() != 3)
        return;
      std::vector<string> new_args(3);
      for (int i = 0; i < 3; ++i) {
        if (lambda_case[i] == case_value) {
          new_args = args;
          break;
        }
      }
      llvm::outs() << "new args: " << new_args[0] << ' ' << new_args[1] << ' '
                   << new_args[2] << '\n';
      auto new_body = case_body_str.substr(0, op_pos + 10) + new_args[0] + ',' +
                      new_args[1] + ',' + new_args[2] +
                      case_body_str.substr(op_end);
      llvm::outs() << "new body: " << new_body << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(case_body->getSourceRange()),
          new_body);
    } else if (auto *IS =
                   Result.Nodes.getNodeAs<clang::SwitchStmt>("Invoke")) {
      if (!IS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IS->getBeginLoc()))
        return;
      auto switch_body = IS->getBody();
      auto switch_body_str =
          stringutils::rangetoStr(*(Result.SourceManager),
                                  switch_body->getSourceRange());
      llvm::outs() << "switch body: " << switch_body_str << '\n';
      if (switch_body_str.find("operator()") == string::npos)
        return;
      auto op_pos = switch_body_str.find("operator()");
      auto op_end = switch_body_str.find('>', op_pos);
      if (op_end == string::npos)
        return;
      auto op_args =
          switch_body_str.substr(op_pos + 10, op_end - op_pos - 10);
      llvm::outs() << "operator args: " << op_args << '\n';
      std::vector<string> args;
      auto arg_pos = op_args.find(',');
      while (arg_pos != string::npos) {
        args.push_back(op_args.substr(0, arg_pos));
        op_args = op_args.substr(arg_pos + 1);
        arg_pos = op_args.find(',');
      }
      args.push_back(op_args);
      llvm::outs() << "args size: " << args.size() << '\n';
      if (args.size() != 3)
        return;
      auto lambda_index = lambda_cases.size() - 1;
      auto lambda_case = lambda_cases[lambda_index];
      if (lambda_case.size() != 3)
        return;
      std::vector<string> new_args(3);
      for (int i = 0; i < 3; ++i) {
        if (lambda_case[i] == args[0]) {
          new_args = lambda_case;
          break;
        }
      }
      llvm::outs() << "new args: " << new_args[0] << ' ' << new_args[1] << ' '
                   << new_args[2] << '\n';
      auto new_body = switch_body_str.substr(0, op_pos + 10) + new_args[0] +
                      ',' + new_args[1] + ',' + new_args[2] +
                      switch_body_str.substr(op_end);
      llvm::outs() << "new body: " << new_body << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(switch_body->getSourceRange()),
          new_body);
    }
}
  
void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambda");
    auto case_matcher = caseStmt().bind("Case");
    auto invoke_matcher = switchStmt().bind("Invoke");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(case_matcher, &callback);
    matchFinder.addMatcher(invoke_matcher, &callback);
    matchFinder.matchAST(Context);
}