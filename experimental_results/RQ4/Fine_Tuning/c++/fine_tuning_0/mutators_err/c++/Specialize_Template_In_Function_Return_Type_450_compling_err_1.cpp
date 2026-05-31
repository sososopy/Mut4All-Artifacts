//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_In_Function_Return_Type_450
 */ 
class MutatorFrontendAction_450 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(450)

private:
    class MutatorASTConsumer_450 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_450(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Specialize_Template_In_Function_Return_Type_450.h"

// ========================================================================================================
#define MUT450_OUTPUT 1

void MutatorFrontendAction_450::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      auto returnType = FD->getReturnType();
      if (!returnType->isStructureOrClassType())
        return;
      auto returnTypeDecl = returnType->getAsCXXRecordDecl();
      if (!returnTypeDecl->isCompleteDefinition())
        return;
      if (!returnTypeDecl->isTemplated())
        return;
      auto returnTypeName = returnType.getAsString();
      llvm::outs() << "return type: " << returnTypeName << '\n';
      auto templateArgs = returnType->getAs<TemplateSpecializationType>();
      if (templateArgs == nullptr)
        return;
      auto templateArg = templateArgs->template_arguments();
      string newReturnType = returnTypeName;
      for (auto arg : templateArg) {
        if (arg.getKind() == TemplateArgument::ArgKind::Type) {
          auto argType = arg.getAsType();
          if (argType->isStructureOrClassType()) {
            auto argTypeDecl = argType->getAsCXXRecordDecl();
            if (argTypeDecl->isCompleteDefinition() &&
                argTypeDecl->isTemplated()) {
              auto argTypeName = argType.getAsString();
              auto argTemplateArgs =
                  argType->getAs<TemplateSpecializationType>();
              auto argTemplateArg = argTemplateArgs->template_arguments();
              string newArgType = argTypeName;
              for (auto argArg : argTemplateArg) {
                if (argArg.getKind() == TemplateArgument::ArgKind::Integral) {
                  auto argArgValue = argArg.getAsIntegral();
                  newArgType.replace(newArgType.find(argArgValue.toString()),
                                     argArgValue.toString().length(), "11");
                }
              }
              newReturnType.replace(newReturnType.find(argTypeName),
                                    argTypeName.length(), newArgType);
            }
          }
        } else if (arg.getKind() == TemplateArgument::ArgKind::Integral) {
          auto argValue = arg.getAsIntegral();
          newReturnType.replace(newReturnType.find(argValue.toString()),
                                argValue.toString().length(), "11");
        }
      }
      llvm::outs() << "new return type: " << newReturnType << '\n';
      Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), newReturnType);
    }
}
  
void MutatorFrontendAction_450::MutatorASTConsumer_450::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}