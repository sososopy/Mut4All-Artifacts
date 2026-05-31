//header file
#pragma once
#include "Mutator_base.h"

/**
 * Attribute_Argument_Template_Dependent_403
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT403_OUTPUT 1

void MutatorFrontendAction_403::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Attr = Result.Nodes.getNodeAs<clang::Attr>("Attribute")) {
      //Filter nodes in header files
      if (!Attr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Attr->getLocation()))
        return;
      
      // Check if the attribute is attached to a member function of a class template
      auto *Decl = Result.Nodes.getNodeAs<clang::Decl>("Decl");
      if (!Decl) return;
      
      auto *Method = dyn_cast<clang::CXXMethodDecl>(Decl);
      if (!Method) return;
      
      auto *Record = dyn_cast<clang::CXXRecordDecl>(Method->getParent());
      if (!Record || !Record->getDescribedClassTemplate()) return;
      
      // Get template parameter
      auto *Template = Record->getDescribedClassTemplate();
      if (!Template || Template->getTemplateParameters()->size() == 0) return;
      auto *Param = Template->getTemplateParameters()->getParam(0);
      if (!Param) return;
      
      // Get attribute arguments
      auto Args = Attr->getArgs();
      if (Args.size() == 0) return;
      
      // Find argument expression that uses template parameter or is constant
      for (unsigned i = 0; i < Args.size(); ++i) {
        auto *Expr = Args[i];
        if (!Expr) continue;
        
        // Check if expression uses template parameter T
        bool usesT = false;
        if (auto *DRE = dyn_cast<clang::DeclRefExpr>(Expr)) {
          if (DRE->getDecl() == Param) usesT = true;
        }
        
        // Check if expression is a simple integer constant
        bool isConstant = false;
        if (auto *IL = dyn_cast<clang::IntegerLiteral>(Expr)) {
          isConstant = true;
        }
        
        if (usesT || isConstant) {
          //Get the source code text of target node
          std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), Expr->getSourceRange());
          
          //Perform mutation on the source code text by applying string replacement
          std::string MutatedText;
          if (usesT) {
            // If argument already uses T, wrap it in (9, (void)T{})
            MutatedText = "(9, (void)T{})";
          } else if (isConstant) {
            // If argument is constant, replace with (void)T{}
            MutatedText = "(void)T{}";
          }
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(Expr->getSourceRange()), MutatedText);
          break; // Mutate only one argument
        }
      }
    }
}
  
void MutatorFrontendAction_403::MutatorASTConsumer_403::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = attr(hasParent(decl().bind("Decl"))).bind("Attribute");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}