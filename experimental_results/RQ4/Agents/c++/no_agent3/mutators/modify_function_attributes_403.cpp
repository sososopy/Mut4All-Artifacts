//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Attributes_403
 */ 
class MutatorFrontendAction_403 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(403)

private:
    class MutatorASTConsumer_403 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_403(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_function_attributes_403.h"

// ========================================================================================================
#define MUT403_OUTPUT 1

void MutatorFrontendAction_403::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithAttributes")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto attrList = FD->getAttrs();
      for (auto *attr : attrList) {
        if (auto *annotateAttr = dyn_cast<AnnotateAttr>(attr)) {
          std::string annotation = annotateAttr->getAnnotation().str();
          if (annotation.find("note") != std::string::npos) {
            //Perform mutation on the source code text by applying string replacement
            std::string mutatedAnnotation = "/*mut403*/" + annotation + " modified";
            Rewrite.ReplaceText(annotateAttr->getLocation(), annotation.size(), mutatedAnnotation);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_403::MutatorASTConsumer_403::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(attr::Annotate)).bind("FunctionWithAttributes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}