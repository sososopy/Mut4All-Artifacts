//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_This_In_Static_Noexcept_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> template_classes;
    };
};

//source file
#include "../include/Mutator_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isStatic())
        return;
      const clang::CXXRecordDecl *recordDecl = MT->getParent();
      if (!recordDecl || !recordDecl->isTemplateDecl())
        return;
      //Get the source code text of target node
      auto methodDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Find a member (field or method) in the class to use in noexcept expression
      std::string memberName = "";
      for (auto decl : recordDecl->decls()) {
        if (auto fieldDecl = dyn_cast<clang::FieldDecl>(decl)) {
          memberName = fieldDecl->getNameAsString();
          break;
        }
        if (auto methodDecl = dyn_cast<clang::CXXMethodDecl>(decl)) {
          if (!methodDecl->isStatic()) {
            memberName = methodDecl->getNameAsString();
            break;
          }
        }
      }
      if (memberName.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string noexceptExpr = "noexcept(this->" + memberName + ")";
      if (MT->getExceptionSpecType() == clang::ExceptionSpecificationType::EST_Unevaluated ||
          MT->getExceptionSpecType() == clang::ExceptionSpecificationType::EST_Dynamic) {
        // Replace existing noexcept specifier
        size_t noexceptPos = methodDecl.find("noexcept");
        if (noexceptPos != std::string::npos) {
          size_t endPos = methodDecl.find(")", noexceptPos);
          if (endPos != std::string::npos) {
            methodDecl.replace(noexceptPos, endPos - noexceptPos + 1, noexceptExpr);
          }
        }
      } else {
        // Add noexcept specifier if absent
        size_t funcEnd = methodDecl.find(")");
        if (funcEnd != std::string::npos) {
          methodDecl.insert(funcEnd + 1, " " + noexceptExpr);
        }
      }
      methodDecl = "/*mut14*/" + methodDecl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), methodDecl);
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isStatic(), hasParent(cxxRecordDecl(isTemplateDecl()))).bind("StaticMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}