//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Noexcept_Specifier_493
 */ 
class MutatorFrontendAction_493 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(493)

private:
    class MutatorASTConsumer_493 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_493(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Noexcept_Specifier_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if the method already has a noexcept or throw specifier
      if (MT->hasAttr<clang::NoThrowAttr>() || MT->getExceptionSpecType() != clang::EST_None)
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Construct the noexcept expression
      std::string noexceptExpr;
      const clang::CXXRecordDecl *classDecl = MT->getParent();
      if (classDecl && classDecl->isCompleteDefinition()) {
        //Check if the class is a template
        if (classDecl->getDescribedClassTemplate() || MT->getDescribedFunctionTemplate()) {
          //Try to find a dependent type member
          for (auto *decl : classDecl->decls()) {
            if (auto *typeAlias = dyn_cast<clang::TypeAliasDecl>(decl)) {
              noexceptExpr = "std::declval<" + typeAlias->getNameAsString() + ">()";
              break;
            } else if (auto *templateParam = dyn_cast<clang::TemplateTypeParmDecl>(decl)) {
              noexceptExpr = "std::declval<" + templateParam->getNameAsString() + ">()";
              break;
            }
          }
          if (noexceptExpr.empty()) {
            //Fallback to a simple expression
            noexceptExpr = "(1, 2)";
          }
        } else {
          //Non-template class: look for a data member
          for (auto *field : classDecl->fields()) {
            if (field->getAccess() == clang::AS_public || field->getAccess() == clang::AS_protected) {
              noexceptExpr = field->getNameAsString();
              break;
            }
          }
          if (noexceptExpr.empty()) {
            //Look for a static member function
            for (auto *method : classDecl->methods()) {
              if (method->isStatic()) {
                noexceptExpr = method->getNameAsString() + "()";
                break;
              }
            }
          }
          if (noexceptExpr.empty()) {
            //Fallback to a simple expression
            noexceptExpr = "(1, 2)";
          }
        }
      } else {
        //No class context available
        noexceptExpr = "(1, 2)";
      }
      //Perform mutation on the source code text by applying string replacement
      //Find the position to insert noexcept specifier
      size_t insertPos = declaration.find(')');
      if (insertPos == std::string::npos) return;
      //Preserve any existing cv-qualifiers
      std::string cvQualifiers;
      if (MT->isConst()) cvQualifiers += " const";
      if (MT->isVolatile()) cvQualifiers += " volatile";
      //Insert noexcept specifier
      std::string mutatedDecl = declaration.substr(0, insertPos + 1) + cvQualifiers + " noexcept(noexcept(" + noexceptExpr + "))" + declaration.substr(insertPos + 1);
      mutatedDecl = "/*mut493*/" + mutatedDecl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDecl);
    }
}
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(unless(hasAttr(clang::attr::NoThrow)), unless(hasExceptionSpec())).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}