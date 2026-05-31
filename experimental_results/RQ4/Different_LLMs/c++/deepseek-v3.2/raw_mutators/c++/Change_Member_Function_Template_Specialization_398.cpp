//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Member_Function_Template_Specialization_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> specializations;
    };
};

//source file
#include "../include/Change_Member_Function_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Specialization")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if it's an explicit specialization of a member function template inside class definition
      if (!FD->isTemplateSpecialization() || !FD->getInstantiatedFromMemberTemplate())
        return;
      
      const clang::CXXRecordDecl* RD = FD->getParent();
      if (!RD || !RD->isCompleteDefinition())
        return;
      
      // Check if specialization is defined inside class body
      SourceLocation ClassEnd = RD->getEndLoc();
      SourceLocation SpecLoc = FD->getBeginLoc();
      if (!Result.SourceManager->isBeforeInTranslationUnit(SpecLoc, ClassEnd))
        return;
      
      //Get the source code text of target node
      auto specializationText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      
      // Remove the specialization from class body
      Rewrite.RemoveText(FD->getSourceRange());
      
      // Record specialization for later out-of-line definition
      specializations.push_back(FD);
      
      // Add declaration outside class (just the declaration, not definition)
      // We'll handle the full out-of-line definition in a separate step
    }
    
    // Process recorded specializations after class definitions are matched
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDef")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      
      if (!RD->isCompleteDefinition())
        return;
      
      // For each recorded specialization belonging to this class
      for (const clang::FunctionDecl* FD : specializations) {
        if (FD->getParent() != RD)
          continue;
        
        // Generate out-of-line specialization definition
        std::string outOfLineDef;
        
        // Get class template parameters
        if (const clang::ClassTemplateDecl* CTD = RD->getDescribedClassTemplate()) {
          // Build template parameter list for class
          std::string classTemplateParams;
          const clang::TemplateParameterList* TPL = CTD->getTemplateParameters();
          if (TPL) {
            classTemplateParams = "template<";
            for (unsigned i = 0; i < TPL->size(); ++i) {
              const clang::NamedDecl* ND = TPL->getParam(i);
              if (i > 0) classTemplateParams += ", ";
              if (const clang::TemplateTypeParmDecl* TTP = clang::dyn_cast<clang::TemplateTypeParmDecl>(ND)) {
                classTemplateParams += "class " + TTP->getNameAsString();
              }
              // Handle other template parameter types if needed
            }
            classTemplateParams += ">";
          }
          
          // Build member function template specialization
          outOfLineDef = classTemplateParams + "\n";
          
          // Get function template specialization signature
          std::string funcSig = FD->getNameAsString();
          
          // Get return type
          std::string returnType = FD->getReturnType().getAsString();
          
          // Build full out-of-line definition
          outOfLineDef += "template<> " + returnType + " " + RD->getNameAsString() + "<T>::" + funcSig + "() {\n";
          
          // Get function body from original specialization
          const clang::CompoundStmt* Body = clang::dyn_cast<clang::CompoundStmt>(FD->getBody());
          if (Body) {
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), Body->getSourceRange());
            outOfLineDef += bodyText.substr(1, bodyText.length() - 2); // Remove braces
          }
          outOfLineDef += "\n}\n";
          
          // Insert after class definition
          SourceLocation insertLoc = RD->getEndLoc();
          Rewrite.InsertTextAfterToken(insertLoc, "\n" + outOfLineDef);
        }
      }
      
      // Clear specializations for this class
      specializations.clear();
    }
}
  
void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher specializationMatcher = functionDecl(isExplicitTemplateSpecialization(), 
                                                            hasParent(cxxRecordDecl(isCompleteDefinition()))).bind("Specialization");
    DeclarationMatcher classMatcher = cxxRecordDecl(isCompleteDefinition()).bind("ClassDef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}