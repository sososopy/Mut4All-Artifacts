//source file
#include "../include/Modify_Template_Specialization_Conditions_453.h"

// ========================================================================================================
#define MUT453_OUTPUT 1

void MutatorFrontendAction_453::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;

        // Get the source code text of target node
        auto specializationRange = TS->getSourceRange();
        auto specializationText = stringutils::rangetoStr(*(Result.SourceManager), specializationRange);

        // Perform mutation on the source code text by applying string replacement
        // Introduce an intermediate struct
        std::string intermediateDecl = "struct intermediate { using type = S; };";
        std::string newSpecializationText = specializationText;
        
        // Replace the direct use of S with intermediate
        size_t pos = newSpecializationText.find("specialize<S>");
        if (pos != std::string::npos) {
            newSpecializationText.replace(pos, std::string("specialize<S>").length(), "intermediate");
        }

        // Replace the original AST node with the mutated one
        newSpecializationText = "/*mut453*/" + intermediateDecl + "\n" + newSpecializationText;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(specializationRange), newSpecializationText);
    }
}

void MutatorFrontendAction_453::MutatorASTConsumer_453::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}