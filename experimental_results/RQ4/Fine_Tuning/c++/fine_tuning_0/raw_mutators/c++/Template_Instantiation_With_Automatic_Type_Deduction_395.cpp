//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_With_Automatic_Type_Deduction_395
 */ 
class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)

private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Instantiation_With_Automatic_Type_Deduction_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->getLocation().isMacroID())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto params = DL->getTemplateParameters();
      if (params->size() != 1)
        return;
      auto param = params->getParam(0);
      if (param->getKind() != clang::TemplateDecl::Kind::TemplateTypeParm)
        return;
      auto TTP = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param);
      if (TTP == nullptr)
        return;
      auto func = DL->getTemplatedDecl();
      if (func->getNumParams() != 1)
        return;
      if (func->getReturnType().getAsString() != TTP->getNameAsString())
        return;
      auto func_param = func->getParamDecl(0);
      if (func_param->getType().getAsString() != "auto &&")
        return;
      if (func->doesThisDeclarationHaveABody() == false)
        return;
      auto requires = DL->getRequiresClause();
      if (requires == nullptr)
        return;
      auto requires_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                  requires->getSourceRange());
      llvm::outs() << requires_str << '\n';

      auto new_requires = requires_str.substr(0, requires_str.size() - 1) +
                          "&& std::is_same_v<std::decay_t<decltype(" +
                          func_param->getNameAsString() + ")>, int>)";
      llvm::outs() << new_requires << '\n';
      content.replace(content.find(requires_str), requires_str.size(),
                      new_requires);
      llvm::outs() << content << '\n';
      auto body = func->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      llvm::outs() << body_str << '\n';
      auto new_body_str = body_str.substr(0, body_str.find('{') + 1) +
                          "using U = std::decay_t<decltype(" +
                          func_param->getNameAsString() + ")>;" +
                          body_str.substr(body_str.find('{') + 1);
      llvm::outs() << new_body_str << '\n';
      content.replace(content.find(body_str), body_str.size(), new_body_str);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      cur_templates.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->getNumArgs() != 1)
        return;
      auto callee = DL->getDirectCallee();
      if (callee == nullptr)
        return;
      auto name = callee->getNameAsString();
      for (auto temp : cur_templates) {
        auto templated_decl = temp->getTemplatedDecl();
        if (name == templated_decl->getNameAsString()) {
          auto arg = DL->getArg(0);
          auto arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                 arg->getSourceRange());
          if (arg_str != "42")
            return;
          auto new_arg_str = "/*mut395*/42";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg->getSourceRange()), new_arg_str);
        }
      }
    }
}
  
void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}