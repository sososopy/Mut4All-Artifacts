//header file
#pragma once
#include "Mutator_base.h"

/**
 * Uninitialized_Member_Access_335
 */ 
class MutatorFrontendAction_335 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(335)

private:
    class MutatorASTConsumer_335 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_335(Rewriter &R) : TheRewriter(R) {}
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
        const FieldDecl *TargetField = nullptr;
    };
};

//source file
#include "../include/uninitialized_member_access_335.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Ctor")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;

      // Find the class declaration
      if (const auto *ClassDecl = dyn_cast<CXXRecordDecl>(Ctor->getParent())) {
        for (const auto *Field : ClassDecl->fields()) {
          if (!Field->hasInClassInitializer()) {
            TargetField = Field;
            break;
          }
        }
      }

      if (TargetField) {
        //Get the source code text of target node
        auto ConstructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                       Ctor->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string CommentText = "/* " + TargetField->getType().getAsString() + " " + TargetField->getNameAsString() + "; */\n";
        std::string InitText = TargetField->getNameAsString() + "(0)";

        // Comment out the field declaration and attempt to initialize it
        ConstructorText.insert(ConstructorText.find("{") + 1, InitText + ", ");
        ConstructorText = CommentText + ConstructorText;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Ctor->getSourceRange()), ConstructorText);
      }
    }
}
  
void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Ctor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}