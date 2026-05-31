//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Return_With_Dependent_Base_Function_170
 */ 
class MutatorFrontendAction_170 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(170)

private:
    class MutatorASTConsumer_170 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_170(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> candidate_base_classes;
        std::vector<const clang::CXXMethodDecl *> candidate_base_methods;
    };
};

//source file
#include "../include/replace_return_with_dependent_base_function_170.h"

// ========================================================================================================
#define MUT170_OUTPUT 1

void MutatorFrontendAction_170::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isInTemplateInstantiation())
        return;
      if (!FD->isTemplated())
        return;
      auto parent = FD->getParent();
      if (!parent->isDependentType())
        return;
      if (FD->hasBody() == false)
        return;
      auto ret_type = FD->getReturnType();
      if (!ret_type->isDependentType())
        return;
      llvm::outs() << "Mut170: " << FD->getNameAsString() << '\n';
      auto body = FD->getBody();
      for (auto stmt : body->children()) {
        if (auto RS = dyn_cast<ReturnStmt>(stmt)) {
          auto ret_expr = RS->getRetValue();
          if (ret_expr == nullptr)
            continue;
          auto ret_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ret_expr->getSourceRange());
          for (std::size_t i = 0; i < candidate_base_classes.size(); ++i) {
            if (parent->isDerivedFrom(candidate_base_classes[i])) {
              auto base_name = candidate_base_classes[i]->getNameAsString();
              auto method_name =
                  candidate_base_methods[i]->getNameAsString();
              auto method_type = candidate_base_methods[i]->getType();
              auto method_type_str = method_type.getAsString();
              llvm::outs() << "Mut170: " << base_name << ' ' << method_name
                           << ' ' << method_type_str << '\n';
              auto replacement =
                  "(this->*static_cast<" + method_type_str + ">(&" +
                  base_name + "::" + method_name + "))()";
              replacement = "/*mut170*/" + replacement;
              Rewrite.ReplaceText(ret_expr->getSourceRange(), replacement);
              break;
            }
          }
        }
      }
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isDependentType() == false)
        return;
      candidate_base_classes.push_back(DL);
      llvm::outs() << "Mut170: " << DL->getNameAsString() << '\n';
      for (auto method : DL->methods()) {
        if (method->isImplicit())
          continue;
        if (method->isStatic())
          continue;
        if (method->getAccess() == AccessSpecifier::AS_private)
          continue;
        if (method->isVirtual())
          continue;
        if (method->isConst() == false)
          continue;
        candidate_base_methods.push_back(method);
        break;
      }
    }
}

void MutatorFrontendAction_170::MutatorASTConsumer_170::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}