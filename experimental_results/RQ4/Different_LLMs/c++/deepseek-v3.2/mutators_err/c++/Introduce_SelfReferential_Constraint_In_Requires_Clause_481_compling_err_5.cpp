//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_SelfReferential_Constraint_In_Requires_Clause_481
 */ 
class MutatorFrontendAction_481 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(481)

private:
    class MutatorASTConsumer_481 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_481(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> mutatedFiles;
    };
};

//source file
#include "../include/Mutator_Introduce_SelfReferential_Constraint_In_Requires_Clause_481.h"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateMemberFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template member function with a requires clause
      if (!FD->isTemplateInstantiation() || FD->getTemplatedKind() == FunctionDecl::TK_NonTemplate)
        return;
      const auto *RequiresExpr = FD->getTrailingRequiresClause();
      if (!RequiresExpr)
        return;
      //Get the source code text of target node
      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation FuncLoc = FD->getBeginLoc();
      std::string FilePath = SM.getFilename(FuncLoc).str();
      //Ensure we only mutate once per file to avoid conflicts
      if (mutatedFiles.find(FilePath) != mutatedFiles.end())
        return;
      
      //Check if <type_traits> is included
      bool hasTypeTraits = false;
      Preprocessor &PP = Result.Context->getPreprocessorPtr()->getPreprocessor();
      PreprocessingRecord *PPRec = PP.getPreprocessingRecord();
      if (PPRec) {
        for (PreprocessedEntity *PE : PPRec->getPreprocessedEntitiesInRange(SourceRange())) {
          if (auto *ID = dyn_cast<clang::InclusionDirective>(PE)) {
            std::string IncludedFile = SM.getFilename(ID->getSourceRange().getBegin()).str();
            if (IncludedFile.find("type_traits") != std::string::npos) {
              hasTypeTraits = true;
              break;
            }
          }
        }
      }
      
      //If not, add #include <type_traits> at the top of the file
      if (!hasTypeTraits) {
        SourceLocation FileStart = SM.getLocForStartOfFile(SM.getFileID(FuncLoc));
        Rewrite.InsertText(FileStart, "#include <type_traits>\n");
        mutatedFiles.insert(FilePath);
      }
      
      //Get the template parameter name
      const TemplateParameterList *TPL = FD->getTemplateSpecializationInfo()->getTemplate()->getTemplateParameters();
      if (!TPL || TPL->size() == 0)
         return;
      const NamedDecl *FirstParam = TPL->getParam(0);
      std::string ParamName = FirstParam->getNameAsString();
      
      //Get the requires clause source range
      SourceRange RequiresRange = RequiresExpr->getSourceRange();
      std::string RequiresText = stringutils::rangetoStr(SM, RequiresRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string NewRequires = "std::is_constructible_v<std::decay_t<" + ParamName + ">, " + ParamName + ">";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(RequiresRange, NewRequires);
      mutatedFiles.insert(FilePath);
    }
}
  
void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasTrailingReturn()).bind("templateMemberFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}