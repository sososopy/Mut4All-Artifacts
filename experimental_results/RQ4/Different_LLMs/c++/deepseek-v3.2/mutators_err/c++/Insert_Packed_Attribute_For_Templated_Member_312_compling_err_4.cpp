//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Packed_Attribute_For_Templated_Member_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Packed_Attribute_For_Templated_Member_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Ensure struct is a complete definition, not a template struct, and is a struct (not class or union)
      if (!MT->isCompleteDefinition() || MT->isClass() || MT->isUnion() || MT->getDescribedTemplate())
        return;
      //Check if struct already has packed attribute
      if (MT->hasAttr<clang::PackedAttr>())
        return;
      //Check if struct contains at least one template member function
      bool hasTemplateMember = false;
      for (auto *D : MT->decls()) {
        if (auto *FD = dyn_cast<clang::FunctionDecl>(D)) {
          if (FD->isTemplated()) {
            hasTemplateMember = true;
            break;
          }
        }
      }
      if (!hasTemplateMember)
        return;
      //Get the source code text of target node
      auto structText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Insert __attribute__((packed)) before struct keyword
      size_t structPos = structText.find("struct");
      if (structPos != std::string::npos) {
        structText.insert(structPos + 6, " __attribute__((packed))");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), structText);
    }
}
  
void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), unless(isTemplateDeclaration()), has(functionDecl(isTemplateInstantiation()))).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}