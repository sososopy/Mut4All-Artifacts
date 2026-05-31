//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Destructor_With_Default_In_Template_232
 */ 
class MutatorFrontendAction_232 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(232)

private:
    class MutatorASTConsumer_232 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_232(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT232_OUTPUT 1

void MutatorFrontendAction_232::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      //Check if destructor is user-defined (not defaulted or implicit)
      if (DT->isImplicit() || DT->isDefaulted())
        return;
      //Get the containing class
      auto *RD = DT->getParent();
      if (!RD || !RD->isCompleteDefinition())
        return;
      //Check if class is a template
      if (!RD->getDescribedClassTemplate() && !RD->isTemplateDecl())
        return;
      //Check if class has at least one member variable whose type is a template parameter or dependent type
      bool hasDependentMember = false;
      for (auto *Field : RD->fields()) {
        QualType QT = Field->getType();
        if (QT->isDependentType() || QT->isInstantiationDependentType()) {
          hasDependentMember = true;
          break;
        }
      }
      if (!hasDependentMember)
        return;
      //Get the source code text of target node
      auto destructorRange = DT->getSourceRange();
      if (!destructorRange.isValid())
        return;
      //Perform mutation on the source code text by applying string replacement
      string destructorText = stringutils::rangetoStr(*(Result.SourceManager), destructorRange);
      //Replace destructor body with "= default"
      size_t bodyStart = destructorText.find('{');
      if (bodyStart != string::npos) {
        size_t bodyEnd = destructorText.find('}', bodyStart);
        if (bodyEnd != string::npos) {
          destructorText.replace(bodyStart, bodyEnd - bodyStart + 1, " = default;");
        }
      } else {
        //If no body found (maybe declaration only), just append "= default"
        destructorText += " = default;";
      }
      destructorText = "/*mut232*/" + destructorText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(destructorRange), destructorText);
    }
}
  
void MutatorFrontendAction_232::MutatorASTConsumer_232::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl(isUserProvided()).bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}