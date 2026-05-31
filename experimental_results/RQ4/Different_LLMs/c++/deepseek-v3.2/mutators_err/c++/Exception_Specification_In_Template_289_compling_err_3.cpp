//header file
#pragma once
#include "Mutator_base.h"

/**
 * Exception_Specification_In_Template_289
 */ 
class MutatorFrontendAction_289 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(289)

private:
    class MutatorASTConsumer_289 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_289(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_289.h"

// ========================================================================================================
#define MUT289_OUTPUT 1

void MutatorFrontendAction_289::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if function is inside a template
      if (!FD->getDeclContext()->isDependentContext())
        return;
      //Check if function has an exception specification
      auto *funcProtoType = FD->getType()->getAs<clang::FunctionProtoType>();
      if (!funcProtoType) return;
      if (funcProtoType->getExceptionSpecType() == clang::EST_None)
        return;
      //Get the source code text of target node
      auto funcDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Find template parameters
      auto *TemplateDecl = dyn_cast<clang::TemplateDecl>(FD->getDeclContext());
      if (!TemplateDecl) return;
      auto *TemplateParams = TemplateDecl->getTemplateParameters();
      if (!TemplateParams || TemplateParams->size() == 0) return;
      //Choose a random template parameter
      int paramIndex = getrandom::getRandomIndex(TemplateParams->size() - 1);
      auto *Param = TemplateParams->getParam(paramIndex);
      string paramName = Param->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      //Replace exception specification with noexcept(std::is_nothrow_invocable_v<Param>)
      string newExceptionSpec = "noexcept(std::is_nothrow_invocable_v<" + paramName + ">)";
      //Find original exception specification position
      //Replace in source text
      size_t specPos = funcDecl.find("noexcept");
      if (specPos != string::npos) {
          size_t endPos = funcDecl.find(')', specPos);
          if (endPos != string::npos) {
              funcDecl.replace(specPos, endPos - specPos + 1, newExceptionSpec);
          }
      } else {
          //Insert after function name
          size_t nameEnd = funcDecl.find('(');
          if (nameEnd != string::npos) {
              funcDecl.insert(nameEnd, " " + newExceptionSpec);
          }
      }
      //Add helper trait if needed
      string helperTrait = "\nusing nothrow_trait = std::is_nothrow_invocable<" + paramName + ">;\n";
      //Find insertion point in template body
      auto *ParentDecl = dyn_cast<clang::Decl>(FD->getDeclContext());
      if (ParentDecl) {
          auto parentRange = ParentDecl->getSourceRange();
          auto parentText = stringutils::rangetoStr(*(Result.SourceManager), parentRange);
          size_t bodyStart = parentText.find('{');
          if (bodyStart != string::npos) {
              parentText.insert(bodyStart + 1, helperTrait);
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(parentRange), parentText);
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
    }
}
  
void MutatorFrontendAction_289::MutatorASTConsumer_289::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasExceptionSpec(), isInstantiated()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}