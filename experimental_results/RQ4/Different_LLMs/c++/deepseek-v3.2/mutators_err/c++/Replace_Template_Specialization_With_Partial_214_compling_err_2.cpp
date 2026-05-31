//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Partial_214
 */ 
class MutatorFrontendAction_214 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(214)

private:
    class MutatorASTConsumer_214 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_214(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
        std::vector<const clang::ClassTemplateSpecializationDecl *> specializationDecls;
    };
};

//source file
#include "../include/Mutator_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(CTD);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("SpecializationDecl")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      if (!CTSD->isExplicitSpecialization())
        return;
      //Record the node information to be used in the mutation process
      specializationDecls.push_back(CTSD);
    }
    else if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TargetSpecialization")) {
      //Filter nodes in header files
      if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTSD->getLocation()))
        return;
      if (!CTSD->isExplicitSpecialization())
        return;
      //Get the source code text of target node
      auto specializationSource = stringutils::rangetoStr(*(Result.SourceManager), CTSD->getSourceRange());
      //Find matching template declaration
      const clang::ClassTemplateDecl *matchingTemplate = nullptr;
      for (auto *CTD : templateDecls) {
        if (CTD->getQualifiedNameAsString() == CTSD->getQualifiedNameAsString()) {
          matchingTemplate = CTD;
          break;
        }
      }
      if (!matchingTemplate)
        return;
      //Find other specializations for the same template
      const clang::ClassTemplateSpecializationDecl *otherSpecialization = nullptr;
      for (auto *spec : specializationDecls) {
        if (spec->getQualifiedNameAsString() == CTSD->getQualifiedNameAsString() && spec != CTSD) {
          otherSpecialization = spec;
          break; 
        }
      }
      if (!otherSpecialization)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string partialSpecializationText;
      const clang::TemplateArgumentList &args = CTSD->getTemplateArgs();
      if (args.size() == 1) {
        //Single parameter: create pointer specialization
        partialSpecializationText = "template<typename T>\nclass " + matchingTemplate->getNameAsString() + "<T*> {\n";
        //Copy body from other specialization
        auto otherSource = stringutils::rangetoStr(*(Result.SourceManager), otherSpecialization->getSourceRange());
        //Extract body between braces
        size_t start = otherSource.find('{');
        size_t end = otherSource.rfind('}');
        if (start != std::string::npos && end != std::string::npos && end > start) {
          std::string body = otherSource.substr(start + 1, end - start - 1);
          partialSpecializationText += body + "\n};\n";
        } else {
          partialSpecializationText += "/*mut214*/int value;\n};\n";
        }
      } else {
        //Multiple parameters: specialize first parameter
        partialSpecializationText = "template<typename T";
        for (unsigned i = 1; i < args.size(); ++i) {
          partialSpecializationText += ", typename U" + std::to_string(i);
        }
        partialSpecializationText += ">\nclass " + matchingTemplate->getNameAsString() + "<int";
        for (unsigned i = 1; i < args.size(); ++i) {
          partialSpecializationText += ", U" + std::to_string(i);
        }
        partialSpecializationText += "> {\n";
        //Copy body from other specialization
        auto otherSource = stringutils::rangetoStr(*(Result.SourceManager), otherSpecialization->getSourceRange());
        size_t start = otherSource.find('{');
        size_t end = otherSource.rfind('}');
        if (start != std::string::npos && end != std::string::npos && end > start) {
          std::string body = otherSource.substr(start + 1, end - start - 1);
          partialSpecializationText += body + "\n};\n";
        } else {
          partialSpecializationText += "/*mut214*/int value;\n};\n";
        }
      }
      //Insert partial specialization after template declaration but before this specialization
      SourceLocation insertLoc = matchingTemplate->getEndLoc();
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut214*/" + partialSpecializationText);
    }
}
  
void MutatorFrontendAction_214::MutatorASTConsumer_214::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher specializationMatcher = classTemplateSpecializationDecl().bind("SpecializationDecl");
    DeclarationMatcher targetMatcher = classTemplateSpecializationDecl(isExplicitSpecialization()).bind("TargetSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.matchAST(Context);
}