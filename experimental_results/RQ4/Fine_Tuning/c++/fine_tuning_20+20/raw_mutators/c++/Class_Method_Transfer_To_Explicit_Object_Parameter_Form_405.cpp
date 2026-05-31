//header file
#pragma once
#include "Mutator_base.h"

/**
 * Class_Method_Transfer_To_Explicit_Object_Parameter_Form_405
 */ 
class MutatorFrontendAction_405 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(405)

private:
    class MutatorASTConsumer_405 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_405(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Class_Method_Transfer_To_Explicit_Object_Parameter_Form_405.h"

// ========================================================================================================
#define MUT405_OUTPUT 1

void MutatorFrontendAction_405::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isStatic())
        return;
      if (MT->isVirtual())
        return;
      if (MT->isLambdaStaticInvoker())
        return;
      if (MT->isCopyAssignmentOperator() || MT->isMoveAssignmentOperator())
        return;
      if (MT->getParent()->isLambda())
        return;
      if (MT->getDescribedFunctionTemplate() != nullptr)
        return;
      if (MT->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (MT->getTemplatedKind() !=
          FunctionDecl::TemplatedKind::TK_NonTemplate)
        return;
      auto method_name = MT->getNameAsString();
      llvm::outs() << method_name << '\n';
      auto parent_name = MT->getParent()->getNameAsString();
      llvm::outs() << parent_name << '\n';
      std::string template_part = "";
      if (MT->getParent()->isStruct() || MT->getParent()->isClass()) {
        if (MT->getParent()->isStruct())
          template_part = "struct ";
        else
          template_part = "class ";
        if (MT->getParent()->getDescribedClassTemplate() != nullptr) {
          template_part += parent_name;
          template_part += "<";
          auto TP = MT->getParent()->getDescribedClassTemplate();
          auto params = TP->getTemplateParameters();
          std::vector<string> param_names;
          for (auto param : *params) {
            param_names.push_back(param->getNameAsString());
          }
          for (std::size_t i = 0; i < param_names.size(); ++i) {
            if (i != 0)
              template_part += ",";
            template_part += param_names[i];
          }
          template_part += ">";
        } else {
          template_part += parent_name;
        }
      }
      llvm::outs() << template_part << '\n';
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      if (declaration.find("this") != string::npos)
        return;
      if (declaration.find("operator") != string::npos)
        return;
      auto pos = declaration.find('(');
      if (pos == string::npos)
        return;
      auto replace_name = method_name + "(this " + template_part + "& self";
      declaration.replace(pos, method_name.size() + 1, replace_name);
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          declaration);
    }
}
  
void MutatorFrontendAction_405::MutatorASTConsumer_405::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}