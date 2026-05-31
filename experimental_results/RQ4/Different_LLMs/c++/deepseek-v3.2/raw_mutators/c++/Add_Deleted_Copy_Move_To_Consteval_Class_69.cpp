//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Deleted_Copy_Move_To_Consteval_Class_69
 */ 
class MutatorFrontendAction_69 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(69)

private:
    class MutatorASTConsumer_69 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_69(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Add_Deleted_Copy_Move_To_Consteval_Class_69.h"

// ========================================================================================================
#define MUT69_OUTPUT 1

void MutatorFrontendAction_69::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ConstevalClass")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition())
        return;
      if (CD->isLambda())
        return;

      bool hasConsteval = false;
      for (auto *MD : CD->methods()) {
        if (MD->isConsteval()) {
          hasConsteval = true;
          break;
        }
      }
      if (!hasConsteval)
        return;

      bool hasDeletedCopyCtor = false;
      bool hasDeletedMoveCtor = false;
      for (auto *Ctor : CD->ctors()) {
        if (Ctor->isCopyConstructor() && Ctor->isDeleted())
          hasDeletedCopyCtor = true;
        if (Ctor->isMoveConstructor() && Ctor->isDeleted())
          hasDeletedMoveCtor = true;
      }

      //Get the source code text of target node
      std::string classText = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
      std::string insertion;

      if (!hasDeletedCopyCtor) {
        insertion += "\n    " + CD->getNameAsString() + "(const " + CD->getNameAsString() + "&) = delete;";
      }
      if (!hasDeletedMoveCtor) {
        insertion += "\n    " + CD->getNameAsString() + "(" + CD->getNameAsString() + "&&) = delete;";
      }

      if (insertion.empty())
        return;

      insertion = "/*mut69*/" + insertion;
      //Perform mutation on the source code text by applying string replacement
      size_t insertPos = classText.rfind('}');
      if (insertPos != std::string::npos) {
        classText.insert(insertPos, insertion);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), classText);
    }
}
  
void MutatorFrontendAction_69::MutatorASTConsumer_69::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasMethod(hasAttr(clang::attr::Consteval))).bind("ConstevalClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}