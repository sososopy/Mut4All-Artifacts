//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Friend_Injection_With_Contradictory_Static_Assertions_179
 */ 
class MutatorFrontendAction_179 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(179)

private:
    class MutatorASTConsumer_179 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_179(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl*> mutatedClasses;
    };
};

//source file
#include "../include/Mutator_Template_Friend_Injection_With_Contradictory_Static_Assertions_179.h"

// ========================================================================================================
#define MUT179_OUTPUT 1

void MutatorFrontendAction_179::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("friendFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if this is a templated friend function defined inside a class template
      auto *Parent = dyn_cast<clang::CXXRecordDecl>(FD->getDeclContext());
      if (!Parent || !Parent->getDescribedClassTemplate() || !FD->isDefined())
        return;
      
      // Ensure we only mutate each class template once
      if (mutatedClasses.find(Parent) != mutatedClasses.end())
        return;
      
      // Check if friend function has a template parameter list
      auto *FT = FD->getDescribedFunctionTemplate();
      if (!FT)
        return;
      
      // Get the function body
      Stmt *Body = FD->getBody();
      if (!Body)
        return;
      
      // Get source range of the function body (excluding braces)
      SourceRange BodyRange = Body->getSourceRange();
      SourceManager &SM = Rewrite.getSourceMgr();
      const LangOptions &LO = Result.Context->getLangOpts();
      
      // Get the template parameters
      auto *TPL = FT->getTemplateParameters();
      if (!TPL || TPL->size() == 0)
        return;
      
      // Check for parameter pack
      bool hasParameterPack = false;
      std::string PackName;
      for (unsigned i = 0; i < TPL->size(); ++i) {
        auto *TP = TPL->getParam(i);
        if (TP->isParameterPack()) {
          hasParameterPack = true;
          PackName = TP->getNameAsString();
          break;
        }
      }
      
      // Prepare condition based on template parameters
      std::string Condition;
      if (hasParameterPack && !PackName.empty()) {
        Condition = "sizeof...(" + PackName + ") > 0";
      } else {
        // Use first template parameter
        auto *FirstTP = TPL->getParam(0);
        if (FirstTP->getKind() == TemplateArgument::Type) {
          Condition = "sizeof(" + FirstTP->getNameAsString() + ") > 0";
        } else {
          Condition = "true";
        }
      }
      
      // Prepare the static assertions and function call
      std::string Insertion = "\n        static_assert(" + Condition + ", \"condition true\");\n";
      Insertion += "        static_assert(!(" + Condition + "), \"condition false\");\n";
      
      // Look for an existing function call in the body to use
      bool foundCall = false;
      for (auto *Child : Body->children()) {
        if (auto *CE = dyn_cast<CallExpr>(Child)) {
          if (auto *Callee = CE->getDirectCallee()) {
            if (Callee->getDescribedFunctionTemplate()) {
              // Found a templated function call, we'll insert after it
              foundCall = true;
              break;
            }
          }
        }
      }
      
      if (!foundCall) {
        // Add a dummy function call using template parameter
        if (hasParameterPack) {
          Insertion += "        dummy<sizeof...(" + PackName + ")>();\n";
        } else {
          Insertion += "        dummy<0>();\n";
        }
      }
      
      // Insert at the beginning of the function body
      SourceLocation InsertLoc = BodyRange.getBegin().getLocWithOffset(1);
      Rewrite.InsertText(InsertLoc, Insertion);
      
      // Mark this class as mutated
      mutatedClasses.insert(Parent);
    }
}
  
void MutatorFrontendAction_179::MutatorASTConsumer_179::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition(),
                                              hasParent(cxxRecordDecl(hasDescendant(classTemplateDecl()))),
                                              isTemplateInstantiation(),
                                              isFriend()).bind("friendFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}