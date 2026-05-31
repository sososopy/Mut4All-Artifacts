//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Constrained_Variable_Template_118
 */ 
class MutatorFrontendAction_118 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(118)

private:
    class MutatorASTConsumer_118 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_118(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> concepts;
    };
};

//source file
#include "../include/Insert_Constrained_Variable_Template_118.h"

// ========================================================================================================
#define MUT118_OUTPUT 1

void MutatorFrontendAction_118::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplates")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->isThisDeclarationADefinition() == VarTemplateDecl::TK_Definition)
        return;
      if (VD->isConstrainedTemplate())
        return;
      llvm::outs() << "Mut118: Found a non-constrained variable template: "
                   << VD->getNameAsString() << '\n';
      auto params = VD->getTemplateParameters();
      if (params->size() != 1)
        return;
      auto param = params->getParam(0);
      if (!param->isTemplateParameter())
        return;
      auto TTP = dyn_cast<TemplateTypeParmDecl>(param);
      if (!TTP)
        return;
      auto conceptname = "AlwaysTrue";
      auto concept = std::find_if(concepts.begin(), concepts.end(),
                                  [&](const ConceptDecl *CD) {
                                    return CD->getNameAsString() == conceptname;
                                  });
      if (concept == concepts.end()) {
        llvm::outs() << "Mut118: Concept " << conceptname
                     << " not found, creating it\n";
        // Insert a concept definition before the variable template
        auto concept_def = std::string("template <typename T> concept ") + conceptname +
                           " = true;\n/*mut118*/";
        Rewrite.ReplaceText(VD->getBeginLoc(), 0, concept_def);
      }
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getSourceRange());
      llvm::outs() << "Mut118: Original declaration: " << decl << '\n';
      auto pos = decl.find('>');
      if (pos == string::npos)
        return;
      decl.insert(pos + 1, std::string(" requires(") + conceptname + "<T>)");
      llvm::outs() << "Mut118: Mutated declaration: " << decl << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()),
                          decl);
    } else if (auto *CD =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      concepts.push_back(CD);
    }
}
  
void MutatorFrontendAction_118::MutatorASTConsumer_118::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::varTemplateDecl().bind("VarTemplates");
    auto concept_matcher = clang::ast_matchers::conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}