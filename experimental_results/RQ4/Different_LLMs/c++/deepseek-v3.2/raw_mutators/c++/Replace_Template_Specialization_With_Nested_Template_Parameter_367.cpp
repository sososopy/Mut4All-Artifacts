//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Nested_Template_Parameter_367
 */ 
class MutatorFrontendAction_367 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(367)

private:
    class MutatorASTConsumer_367 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_367(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> existingWrapperDecls;
    };
};

//source file
#include "../include/Replace_Template_Specialization_With_Nested_Template_Parameter_367.h"

// ========================================================================================================
#define MUT367_OUTPUT 1

void MutatorFrontendAction_367::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionSpecialization")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      // Ensure it's a template specialization
      if (!FS->isTemplateInstantiation() || FS->getTemplateSpecializationKind() == clang::TSK_Undeclared)
        return;
      // Get the primary template
      const clang::FunctionTemplateDecl *PrimaryTemplate = FS->getPrimaryTemplate();
      if (!PrimaryTemplate)
        return;
      // Check if it's a specialization with exactly one template parameter
      const clang::TemplateArgumentList *Args = FS->getTemplateSpecializationArgs();
      if (!Args || Args->size() != 1)
        return;
      // Get the specialized type
      const clang::TemplateArgument &Arg = Args->get(0);
      if (Arg.getKind() != clang::TemplateArgument::Type)
        return;
      clang::QualType SpecializedType = Arg.getAsType();
      // Find a wrapper template class in the existing declarations
      clang::ClassTemplateDecl *WrapperDecl = nullptr;
      for (auto *Decl : existingWrapperDecls) {
        if (Decl->getTemplateParameters()->size() == 1) {
          WrapperDecl = Decl;
          break;
        }
      }
      // If no suitable wrapper exists, create one before the specialization
      if (!WrapperDecl) {
        std::string WrapperCode = "template<typename U> struct Wrapper {};\n";
        SourceLocation InsertLoc = FS->getBeginLoc();
        Rewrite.InsertTextBefore(InsertLoc, WrapperCode);
        // After insertion, we need to find the newly inserted wrapper
        // For simplicity, we assume it's now available and proceed
        // We'll just use the name "Wrapper" in the mutation
      }
      // Get the function's parameter list
      if (FS->getNumParams() == 0)
        return;
      clang::ParmVarDecl *FirstParam = FS->getParamDecl(0);
      clang::QualType ParamType = FirstParam->getType();
      // Check if the parameter type matches the specialized type (or is derived from it)
      // For simplicity, we assume the first parameter uses the specialized type directly
      // Get the source range of the parameter type
      SourceRange ParamTypeRange = FirstParam->getSourceRange();
      // We need to find the exact location of the type in the parameter
      // This is complex; we'll approximate by using the parameter's start location
      // Actually, we need to replace the type in the function signature
      // Let's get the function's declaration source
      std::string FuncDecl = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());
      // Find the parameter type substring in the declaration
      // This is heuristic; we'll replace the specialized type name with Wrapper<type>
      std::string TypeName = SpecializedType.getAsString();
      // Find the type name in the function declaration (approximate)
      size_t Pos = FuncDecl.find(TypeName);
      if (Pos == std::string::npos)
        return;
      // Perform mutation on the source code text by applying string replacement
      std::string NewTypeName = "Wrapper<" + TypeName + ">";
      FuncDecl.replace(Pos, TypeName.length(), NewTypeName);
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), FuncDecl);
    }
    // Also collect existing template class declarations for wrapper candidates
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      existingWrapperDecls.push_back(CTD);
    }
}
  
void MutatorFrontendAction_367::MutatorASTConsumer_367::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl(isTemplateInstantiation(), hasTemplateSpecializationArgs()).bind("FunctionSpecialization");
    DeclarationMatcher classMatcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}