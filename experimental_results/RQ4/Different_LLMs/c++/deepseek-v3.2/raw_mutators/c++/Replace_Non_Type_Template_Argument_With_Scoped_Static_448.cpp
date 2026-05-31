//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Non_Type_Template_Argument_With_Scoped_Static_448
 */ 
class MutatorFrontendAction_448 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(448)

private:
    class MutatorASTConsumer_448 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_448(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> candidateVariables;
    };
};

//source file
#include "../include/Mutator_Replace_Non_Type_Template_Argument_With_Scoped_Static_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record candidate variables that are at namespace scope or static members
      if (VD->hasGlobalStorage() && VD->getType()->isIntegralOrEnumerationType() || VD->getType()->isPointerType() || VD->getType()->isReferenceType()) {
        candidateVariables.push_back(VD);
      }
    }
    else if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateArgument>("TemplateArgument")) {
      //Filter nodes in header files
      if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getLocation()))
        return;
      //Get the source code text of target node
      if (TE->getKind() != clang::TemplateArgument::ArgKind::Integral && TE->getKind() != clang::TemplateArgument::ArgKind::Declaration)
        return;
      
      //Find a candidate variable that matches the template argument type
      const clang::VarDecl* matchingVar = nullptr;
      for (const clang::VarDecl* VD : candidateVariables) {
        if (TE->getKind() == clang::TemplateArgument::ArgKind::Integral) {
          if (VD->getType()->isIntegralOrEnumerationType()) {
            matchingVar = VD;
            break;
          }
        } else if (TE->getKind() == clang::TemplateArgument::ArgKind::Declaration) {
          if (VD->getType()->isPointerType() || VD->getType()->isReferenceType()) {
            matchingVar = VD;
            break;
          }
        }
      }
      
      if (!matchingVar)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      //Create a new static variable inside a function
      std::string newStaticVar = "static " + matchingVar->getType().getAsString() + " scoped_static_" + matchingVar->getNameAsString() + " = ";
      if (TE->getKind() == clang::TemplateArgument::ArgKind::Integral) {
        newStaticVar += std::to_string(TE->getAsIntegral().getExtValue());
      } else {
        newStaticVar += matchingVar->getNameAsString();
      }
      newStaticVar += ";";
      
      //Wrap the template instantiation inside a new function if not already in a function
      clang::SourceLocation loc = TE->getLocation();
      clang::SourceManager& SM = Rewrite.getSourceMgr();
      clang::DeclContext* DC = Result.Context->getDeclContextForLocation(loc);
      bool isInFunction = DC && (clang::isa<clang::FunctionDecl>(DC) || clang::isa<clang::MethodDecl>(DC));
      
      std::string mutation;
      if (!isInFunction) {
        mutation = "void mutator_function_448() {\n    " + newStaticVar + "\n    ";
        //Need to get the full template instantiation text
        clang::SourceRange range = TE->getSourceRange();
        std::string templateText = stringutils::rangetoStr(*Result.SourceManager, range);
        mutation += templateText + "\n}\n";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(range, mutation);
      } else {
        mutation = newStaticVar + "\n    ";
        Rewrite.InsertTextBefore(loc, mutation);
      }
    }
}
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl(hasGlobalStorage()).bind("VarDecl");
    DeclarationMatcher templateArgMatcher = templateArgument().bind("TemplateArgument");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(templateArgMatcher, &callback);
    matchFinder.matchAST(Context);
}