//header file
#pragma once
#include "Mutator_base.h"

/**
 * Refine_Concept_Usage_In_Function_Template_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)

private:
    class MutatorASTConsumer_143 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Refine_Concept_Usage_In_Function_Template_143.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isThisDeclarationADefinition() == false)
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';

      auto func = MT->getTemplatedDecl();
      auto params = func->getTemplateSpecializationArgs();
      if (params == nullptr)
        return;
      auto paramlist = params->asArray();
      llvm::outs() << paramlist.size() << '\n';
      for (auto param : paramlist) {
        if (param.getKind() ==
            TemplateArgument::ArgKind::Type) { // if it is a concept
          auto paramtype = param.getAsType();
          if (paramtype->isDependentType() == true) {
            auto paramtypestr = clang::TypeName::getFullyQualifiedName(
                paramtype, *(Result.Context));
            llvm::outs() << paramtypestr << '\n';
            stringutils::replace_all(content, paramtypestr, "typename");
          }
        } else if (param.getKind() == TemplateArgument::ArgKind::Integral) {
          auto paramtype = param.getAsIntegral();
          llvm::outs() << paramtype << '\n';
        }
      }
      auto func_params = func->parameters();
      for (auto param : func_params) {
        auto param_type = param->getType();
        llvm::outs() << param_type.getAsString() << '\n';
        if (param_type->isDependentType() == true) {
          auto paramtypestr = clang::TypeName::getFullyQualifiedName(
              param_type, *(Result.Context));
          llvm::outs() << paramtypestr << '\n';
          stringutils::replace_all(content, paramtypestr, "auto");
        }
      }
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}