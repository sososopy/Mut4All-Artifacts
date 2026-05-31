//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Unconstrained_Template_Member_With_Constrained_87
 */ 
class MutatorFrontendAction_87 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(87)

private:
    class MutatorASTConsumer_87 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_87(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> outerClassTemplates;
    };
};

//source file
#include "../include/Mutator_Replace_Unconstrained_Template_Member_With_Constrained_87.h"

// ========================================================================================================
#define MUT87_OUTPUT 1

void MutatorFrontendAction_87::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("OuterTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      outerClassTemplates.push_back(CTD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("InnerTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto outerClass = FD->getDeclContext();
      if (!outerClass) return;
      const clang::ClassTemplateDecl* outerTemplate = nullptr;
      for (auto* templ : outerClassTemplates) {
        if (templ->getTemplatedDecl() == outerClass) {
          outerTemplate = templ;
          break;
        }
      }
      if (!outerTemplate) return;
      
      // Check if concept already exists
      bool conceptExists = false;
      for (auto* decl : outerTemplate->getTemplatedDecl()->decls()) {
        if (auto* conceptDecl = dyn_cast<clang::ConceptDecl>(decl)) {
          conceptExists = true;
          break;
        }
      }
      
      // If concept doesn't exist, add it
      if (!conceptExists) {
        string conceptText = "\ntemplate<typename X> concept C = true;\n";
        Rewrite.InsertTextAfterToken(outerTemplate->getBeginLoc(), conceptText);
      }
      
      // Perform mutation on the source code text by applying string replacement
      auto sourceRange = FD->getSourceRange();
      string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      // Find template parameter list
      size_t templatePos = originalText.find("template<");
      if (templatePos == string::npos) return;
      
      // Find function name after template parameters
      size_t funcStart = originalText.find('>', templatePos);
      if (funcStart == string::npos) return;
      
      // Insert requires clause
      string mutatedText = originalText.substr(0, funcStart + 1);
      mutatedText += " requires C<";
      // Get outer template parameter name
      auto outerParams = outerTemplate->getTemplateParameters();
      if (outerParams && outerParams->size() > 0) {
        mutatedText += outerParams->getParam(0)->getNameAsString();
      } else {
        mutatedText += "T";
      }
      mutatedText += ">";
      mutatedText += originalText.substr(funcStart + 1);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutated2);
    }
}
  
void MutatorFrontendAction_87::MutatorASTConsumer_87::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher outerMatcher = classTemplateDecl().bind("OuterTemplate");
    DeclarationMatcher innerMatcher = functionTemplateDecl(hasParent(classTemplateDecl())).bind("InnerTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.addMatcher(innerMatcher, &callback);
    matchFinder.matchAST(Context);
}