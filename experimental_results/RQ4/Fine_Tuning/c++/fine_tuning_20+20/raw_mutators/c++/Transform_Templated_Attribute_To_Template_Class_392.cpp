//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Templated_Attribute_To_Template_Class_392
 */ 
class MutatorFrontendAction_392 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(392)

private:
    class MutatorASTConsumer_392 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_392(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> candidates;
    };
};

//source file
#include "../include/Transform_Templated_Attribute_To_Template_Class_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("using") == string::npos)
      return;
    candidates.push_back(DL);
  } else if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "Structs")) { // if there is a struct with a templated
                               // attribute, transform it into a template class
    if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ST->getLocation()))
      return;
    if (!ST->isStruct())
      return;
    if (!ST->isCompleteDefinition())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
    if (content.find("using") == string::npos)
      return;
    for (auto candidate : candidates) {
      if (ST->getNameAsString() == candidate->getNameAsString()) {
        llvm::outs() << "Mut392: Found a target struct!\n";
        content = "/*mut392*/" + content;
        content = stringutils::strReplace(content, "struct", "template ");
        content = stringutils::strReplace(content, "using", "void");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(ST->getSourceRange()), content);
        break;
      }
    }
  }
}
  
void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}