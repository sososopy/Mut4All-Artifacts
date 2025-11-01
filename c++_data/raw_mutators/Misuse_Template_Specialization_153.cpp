//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Template_Specialization_153
 */ 
class MutatorFrontendAction_153 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(153)

private:
    class MutatorASTConsumer_153 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_153(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Misuse_Template_Specialization_153.h"

// ========================================================================================================
#define MUT153_OUTPUT 1

void MutatorFrontendAction_153::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      for (auto *spec : FT->specializations()) {
        if (spec->getNumParams() > 0) {
          auto *param = spec->getParamDecl(0);
          if (auto *templateType = llvm::dyn_cast<TemplateTypeParmDecl>(param->getType()->getAs<TemplateTypeParmType>()->getDecl())) {
            //Get the source code text of target node
            auto paramType = param->getType().getAsString();
            std::string newParamType = paramType;

            //Perform mutation on the source code text by applying string replacement
            if (paramType.find("const") != std::string::npos) {
              newParamType = paramType.replace(paramType.find("const"), 5, "");
            } else {
              newParamType = "const " + paramType;
            }

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(param->getSourceRange(), newParamType);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_153::MutatorASTConsumer_153::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}