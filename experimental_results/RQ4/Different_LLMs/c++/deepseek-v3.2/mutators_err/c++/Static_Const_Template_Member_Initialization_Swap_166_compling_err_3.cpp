//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Const_Template_Member_Initialization_Swap_166
 */ 
class MutatorFrontendAction_166 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(166)

private:
    class MutatorASTConsumer_166 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_166(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> inlineStaticConstMembers;
        std::vector<const clang::VarDecl *> outOfClassStaticConstMembers;
    };
};

//source file
#include "../include/Mutator_Static_Const_Template_Member_Initialization_Swap_166.h"

// ========================================================================================================
#define MUT166_OUTPUT 1

void MutatorFrontendAction_166::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("inlineStaticConstMember")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if it's a static const member of a class template with inline initializer
      if (VD->isStaticDataMember() && VD->isConstexpr() && VD->hasInit()) {
        if (auto *RD = dyn_cast<CXXRecordDecl>(VD->getDeclContext())) {
          if (RD->getDescribedClassTemplate() || RD->isTemplateDecl()) {
            //Record the node information to be used in the mutation process
            inlineStaticConstMembers.push_back(VD);
          }
        }
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("outOfClassStaticConstMember")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if it's an out-of-class definition of a static const template member
      if (VD->isStaticDataMember() && VD->isConstexpr() && VD->hasInit()) {
        if (auto *TSD = dyn_cast<ClassTemplateSpecializationDecl>(VD->getDeclContext())) {
          // This is an out-of-class definition
          outOfClassStaticConstMembers.push_back(VD);
        }
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("staticConstMemberDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // This handles the transformation from out-of-class to inline
      if (VD->isStaticDataMember() && VD->isConstexpr() && !VD->hasInit()) {
        if (auto *RD = dyn_cast<CXXRecordDecl>(VD->getDeclContext())) {
          if (RD->getDescribedClassTemplate() || RD->isTemplateDecl()) {
            // Find corresponding out-of-class definition
            for (auto *OutVD : outOfClassStaticConstMembers) {
              if (OutVD->getDeclName() == VD->getDeclName()) {
                //Get the source code text of target node
                auto DeclRange = VD->getSourceRange();
                auto Init = OutVD->getInit();
                if (Init) {
                  auto &SM = Rewrite.getSourceMgr();
                  auto InitText = stringutils::rangetoStr(SM, Init->getSourceRange());
                  //Perform mutation on the source code text by applying string replacement
                  std::string NewDecl = stringutils::rangetoStr(SM, DeclRange);
                  NewDecl += " = " + InitText;
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(DeclRange, NewDecl);
                  // Remove the out-of-class definition
                  Rewrite.RemoveText(OutVD->getSourceRange());
                }
                break;
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_166::MutatorASTConsumer_166::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // Matcher for inline static const members in class templates
    DeclarationMatcher inlineMatcher = varDecl(
        allOf(
            hasStaticStorageDuration(),
            isConstexpr(),
            hasInitializer(expr()),
            hasParent(recordDecl(anyOf(
                hasDescendant(classTemplateDecl()),
                isTemplateDecl()
            )))
        )
    ).bind("inlineStaticConstMember");
    
    // Matcher for out-of-class definitions of static const template members
    DeclarationMatcher outOfClassMatcher = varDecl(
        allOf(
            hasStaticStorageDuration(),
            isConstexpr(),
            hasInitializer(expr()),
            hasParent(recordDecl(isInstantiated()))
        )
    ).bind("outOfClassStaticConstMember");
    
    // Matcher for declarations without initializers (for reverse transformation)
    DeclarationMatcher declMatcher = varDecl(
        allOf(
            hasStaticStorageDuration(),
            isConstexpr(),
            unless(hasInitializer(expr())),
            hasParent(recordDecl(anyOf(
                hasDescendant(classTemplateDecl()),
                isTemplateDecl()
            )))
        )
    ).bind("staticConstMemberDecl");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inlineMatcher, &callback);
    matchFinder.addMatcher(outOfClassMatcher, &callback);
    matchFinder.addMatcher(declMatcher, &callback);
    matchFinder.matchAST(Context);
}