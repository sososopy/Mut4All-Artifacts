//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Friend_Auto_Return_Dependency_439
 */ 
class MutatorFrontendAction_439 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(439)

private:
    class MutatorASTConsumer_439 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_439(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> friendFuncs;
        std::vector<const clang::CXXRecordDecl*> enclosingClasses;
    };
};

//source file
#include "../include/Mutator_Friend_Auto_Return_Dependency_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("friendFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a friend function with auto return type
      if (FD->getFriendObjectKind() != clang::Decl::FriendObjectKind::FOK_None || !FD->getReturnType()->getContainedAutoType())
        return;
      
      //Get the enclosing class template
      const clang::CXXRecordDecl* enclosingClass = nullptr;
      auto* DC = FD->getDeclContext();
      for (auto decl = DC->decls().begin(); decl != DC->decls().end(); ++decl) {
        if (auto* RD = dyn_cast<clang::CXXRecordDecl>(*decl)) {
          if (RD->getDescribedClassTemplate()) {
            enclosingClass = RD;
            break;
          }
        }
      }
      if (!enclosingClass)
        return;
      
      //Record node information
      friendFuncs.push_back(FD);
      enclosingClasses.push_back(enclosingClass);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("targetFunc")) {
      //Filter nodes in1 header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      //Find matching friend function in recorded list
      for (size_t i = 0; i < friendFuncs.size(); ++i) {
        if (FD == friendFuncs[i]) {
          const clang::CXXRecordDecl* enclosingClass = enclosingClasses[i];
          
          //Get the source code text of target node
          auto funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
          
          //Check if already has trailing return type
          bool hasTrailingReturn = FD->getReturnType()->getContainedAutoType() && 
                                   FD->getReturnType()->getContainedAutoType()->getDeducedType().isNull();
          
          //Get template parameters
          auto* templateDecl = enclosingClass->getDescribedClassTemplate();
          if (!templateDecl)
            return;
          
          //Choose a template parameter to use
          auto* templateParams = templateDecl->getTemplateParameters();
          if (templateParams->size() == 0)
            return;
          
          size_t paramIndex = getrandom::getRandomIndex(templateParams->size() - 1);
          auto* templateParam = templateParams->getParam(paramIndex);
          
          //Generate new trailing return type expression
          std::string newReturnExpr;
          if (auto* NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(templateParam)) {
            //Non-type template parameter (e.g., int N)
            newReturnExpr = " -> decltype(" + NTTP->getNameAsString() + " - 1 + sizeof(" + NTTP->getNameAsString() + "))";
          } else if (auto* TTP = dyn_cast<clang::TemplateTypeParmDecl>(templateParam)) {
            //Type template parameter
            newReturnExpr = " -> decltype(sizeof(" + TTP->getNameAsString() + "))";
          } else if (auto* TTTP = dyn_cast<clang::TemplateTemplateParmDecl>(templateParam)) {
            //Template template parameter - use simpler expression
            newReturnExpr = " -> decltype(" + TTTP->getNameAsString() + ")";
          }
          
          //Perform mutation on the source code text by applying string replacement
          if (!hasTrailingReturn) {
            //Insert trailing return type before function body
            size_t bodyStart = funcText.find("{");
            if (bodyStart != std::string::npos) {
              funcText.insert(bodyStart, newReturnExpr);
            }
          } else {
            //Replace existing trailing return type
            size_t arrowPos = funcText.find("->");
            if (arrowPos != std::string::npos) {
              size_t arrowEnd = funcText.find("{", arrowPos);
              if (arrowEnd != std::string::npos) {
                funcText.replace(arrowPos, arrowEnd - arrowPos, newReturnExpr);
              }
            }
          }
          
          funcText = "/*mut439*/" + funcText;
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
          
          //Remove from vectors to avoid duplicate mutations
          friendFuncs.erase(friendFuncs.begin() + i);
          enclosingClasses.erase(enclosingClasses.begin() + i);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //First matcher to find friend functions with auto return in template classes
    DeclarationMatcher friendMatcher = functionDecl(
        hasAttr(clang::attr::Friend),
        returns(autoType()),
        hasParent(cxxRecordDecl(hasDescendant(classTemplateDecl())))
    ).bind("friendFunc");
    
    //Second matcher to actually mutate the matched functions
    DeclarationMatcher targetMatcher = functionDecl().bind("targetFunc");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(friendMatcher, &callback);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.matchAST(Context);
}